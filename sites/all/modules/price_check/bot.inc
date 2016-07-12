<?php

function price_check_bot_run_bot($form, &$form_state) {
  $nid = $form['nid']['#value'];
  $node = node_load($nid);
  $node->field_api_usage['und'][0]['value'] = 0;
  $node->field_start_seller_index['und'][0]['value'] = $node->field_seller_index['und'][0]['value'];
  $sellers = price_check_decode_field($node->field_sellers['und']);
  if(!$sellers) {
    price_check_bot_log_message($nid, "No sellers to run.");
    return;
  }
  $seller = $sellers[$node->field_seller_index['und'][0]['value']];
  $queue = DrupalQueue::get('price_check_bot_queue_atom_1'); 
  $queue->createQueue();
  $queue->createItem(array('nid'=> $nid, 'work_item' => $seller, 'op' => 'run_seller')); 
  price_check_bot_log_message($node, "Init seller $seller.");
  node_save($node);
}

function price_check_bot_log_message(&$bot, $message) {
  $message = time() . " $message";
  $sep = "\n\n";
  if(is_numeric($bot)) {
    $node = node_load($bot);
    $node->field_messages['und'][] = array('value' => $message);
    node_save($node);
  } else {
    $bot->field_messages['und'][] = array('value' => $message);
  }
}

function price_check_bot_queue_atom_1($params) {
  price_check_bot_log_message($node, "queue_call: " . print_r($params, true));
  $nid = $params['nid'];
  $work_item = @$params['work_item'];
  $op = $params['op'];
  $end = @$params['end'];
  $node = node_load($nid);
  $uid = $node->uid;
  if($op == 'run_seller') {
    $seller = $work_item;
    $page = 1;
    $min_sales = $node->field_min_sales['und'][0]['value'];
    price_check_bot_log_message($node, "Run seller $seller.");
    $data = price_check_get_ebay_seller_items($seller, $page, $uid);
    $node->field_api_usage['und'][0]['value']++;
    $results = $data['paginationOutput']['totalEntries'];
    $queue = DrupalQueue::get('price_check_bot_queue_atom_1'); 
    $queue->createQueue();
    if(!$results) {
      price_check_bot_log_message($node, print_r($data, true));
      price_check_bot_log_message($node, "$seller has no items.");
      $queue->createItem(array('nid'=> $nid, 'op' => 'continue')); 
      node_save($node);
      return;
    }
    $items = price_check_force_json_array($data['searchResult']['item']);
    foreach($items as $item) {
      price_check_bot_log_message($node, "Test {$item['itemId']}");
      price_check_bot_log_message($node, "Item " . print_r($item, true));
      if($item['isMultiVariationListing'] == 'true') continue;
      if($item['autoPay'] == 'false') continue;
      if($item['sellingStatus']['currentPrice'] + 0 < $node->field_min_price['und'][0]['value'] + 0) continue;
      price_check_bot_log_message($node, "Search {$item['itemId']}");
      $work_item = array('item_id' => $item['itemId'], 'url' => $item['viewItemURL'], 'min_sales' => $min_sales);
      $queue->createItem(array('nid'=> $nid, 'work_item' => $work_item, 'op' => 'run_item'));  
    }
    $queue->createItem(array('nid'=> $nid, 'op' => 'continue')); 
    node_save($node);
    return;
  }
  if($op == 'continue') {
    price_check_bot_log_message($node, "Processing continue.");
    //check limit
    $usage = $node->field_api_usage['und'][0]['value'];
    $limit = $node->field_api_limit['und'][0]['value'];
    if($usage >= $limit) {
      price_check_bot_log_message($node, "Used $usage of $limit. Stopping bot now.");
      node_save($node);
      return;
    }
    $sellers = price_check_decode_field($node->field_sellers['und']);
    $seller_index = $node->field_seller_index['und'][0]['value'];
    $seller_index++;
    if($seller_index >= count($sellers)) $seller_index = 0;
    if($seller_index == $node->field_start_seller_index['und'][0]['value']) {
      price_check_bot_log_message($node, "Finished all sellers.");
      node_save($node);
      return;
    }
    $node->field_seller_index['und'][0]['value'] = $seller_index;
    $seller = $sellers[$seller_index];
    $queue = DrupalQueue::get('price_check_bot_queue_atom_1'); 
    $queue->createQueue();
    $queue->createItem(array('nid'=> $nid, 'work_item' => $seller, 'op' => 'run_seller')); 
    price_check_bot_log_message($node, "Queue seller $seller.");
    node_save($node);
    return;
  }
  if($op == 'run_item') {
    $item_id = $work_item['item_id'];
    $ebay_url = $work_item['url'];
    $min_sales = $work_item['min_sales'];
    price_check_bot_log_message($node, "Try item $item_id.");
    //did we import this before?
    if(price_check_is_paying_customer($uid)) {
      $sql = <<<SQL
select count(*) n from field_data_field_ebay_url fu
join field_data_field_imported fi on fu.entity_id = fi.entity_id
join node on nid = fu.entity_id
where field_ebay_url_value = :ebay_url and
uid = :uid
SQL;
      $result = db_query($sql, array(':ebay_url' => $ebay_url, ':uid' => $uid));
      $n = $result->fetchField();  
      if($n > 0) return; 
    } else {
      $sql = <<<SQL
select count(*) n from field_data_field_ebay_url fu
join field_data_field_imported fi on fu.entity_id = fi.entity_id
where field_ebay_url_value = :ebay_url
SQL;
      $result = db_query($sql, array(':ebay_url' => $ebay_url));
      $n = $result->fetchField();  
      if($n > 0) return; 
    }
    $node->field_api_usage['und'][0]['value']++;
    $sales = price_check_get_item_sales_30($item_id, $uid);
    if($sales < $min_sales) {
      price_check_bot_log_message($node, "Item no min sales $item_id.");
      node_save($node);
      return;
    }
    $node->field_api_usage['und'][0]['value']++;
    $item = price_check_get_ebay_item_from_url($ebay_url, $uid);
    $title = $item['Item']['Title'];
    $description = price_check_strip_tags($item['Item']['Description']);
    $price = $item['Item']['StartPrice'];
    if(!@$item['Item']['ConditionID']) return; 
    $condition = $item['Item']['ConditionID'];
    $category = $item['Item']['PrimaryCategory']['CategoryID'];
    $seller = $item['Item']['Seller']['UserID'];
    $quantity_sold = $sales;
    $images_array = price_check_force_json_array($item['Item']['PictureDetails']['PictureURL']);
    $nid = @price_check_save_item_template_node($ebay_url, $title, $description, $images_array, $response, $price, $condition, $category, $seller, $quantity_sold, $uid);
    $output = "$title - $nid added to list.";
    price_check_bot_log_message($node, $output);
    node_save($node);
    return;
  }
  
}

function price_check_get_bot($uid = false) {
  if(!$uid) {
    global $user;
    $uid = $user->uid;
  }
  $sql = "select nid from node where type = 'bot' and uid = :uid";
  $results = db_query($sql, array('uid' => $uid));
  foreach($results as $row) {
    return node_load($row->nid);
  }
  $node = new stdClass();
  $node->title = 'My Bot';
  $node->type = "bot";
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->uid = $uid; 
  $node->status = 1; 
  $node->promote = 0;
  $node->comment = 1;
  //$node->field_content_access_token[$node->language][0]['value'] = price_check_get_user_content_access_token();
  $node = node_submit($node); 
  node_save($node);
  if(!@$node->nid) return false;
  return $node;
}