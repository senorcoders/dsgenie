<?php

include_once DRUPAL_ROOT . '/sites/all/modules/price_check/pages.dev.inc';

function price_check_ebay_api_info($form, &$form_state) {
  $form['devid'] = array(
    '#type' => 'textfield', 
    '#title' => 'DEVID',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['appid'] = array(
    '#type' => 'textfield', 
    '#title' => 'AppID',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['certid'] = array(
    '#type' => 'textfield', 
    '#title' => 'CertID',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['runame'] = array(
    '#type' => 'textfield', 
    '#title' => 'RuName',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['authtoken'] = array(
    '#type' => 'textfield', 
    '#title' => 'Ebay Auth Token',
    '#size' => 80,
    '#maxlength' => 3000,
    '#required' => TRUE, 
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function price_check_ebay_api_info_submit($form, &$form_state) {  
  $devid = $form_state['values']['devid'];
  $appid = $form_state['values']['appid']; 
  $certid = $form_state['values']['certid']; 
  $runame = $form_state['values']['runame'];
  $authtoken = $form_state['values']['authtoken']; 
  
  global $user;
  $account = user_load($user->uid);
  $account->field_devid['und'][0]['value'] = $devid;
  $account->field_appid['und'][0]['value'] = $appid;
  $account->field_certid['und'][0]['value'] = $certid;
  $account->field_runame['und'][0]['value'] = $runame;
  $account->field_authtoken['und'][0]['value'] = $authtoken;
  user_save($account);
  drupal_set_message('Ebay Api Info updated.');
  drupal_goto('/');
}

function price_check_swap_ebay_item($nid, $ebay_account) {
  $item_template = node_load($nid);
  global $user;
  if($user->uid != $item_template->uid) {
    return "This is not your item.";
  } 
  $subscribers = price_check_decode_field(@$item_template->field_subscribers['und'], true);
  if($subscribers) return "You already listed this one in an eBay Account";
  //check if there are any suitable items to swap
  $sql = <<<SQL
select count(*) num_listings from field_data_field_complete fc 
join field_data_field_imported fi on fi.entity_id = fc.entity_id 
join field_data_field_store_price fp on fi.entity_id = fp.entity_id 
join node on node.nid = fc.entity_id
where field_complete_value = 1 and 
uid in (5,1,2290)
SQL;
  $result = db_query($sql);
  $num_listings = $result->fetchField();
  if(!$num_listings) return "Sorry. No available items to swap at this time.";
  $count = price_check_give_listings($user->uid, 1, true);
  if(!$count) return "An error occurred. Try again later.";
  $item_template->uid = 5;
  $item_template->field_suggested_accounts['und'] = array();
  $item_template->field_complete['und'][0]['value'] = 1;
  node_save($item_template);
  drupal_set_message("The suggestion has been replaced.");
  drupal_goto('/');  
}

function price_check_list_ebay_item($nid, $ebay_account) {
  global $user;
  $item_template = node_load($nid);
  $uid = $user->uid;
  if($uid != $item_template->uid) return "This is not your item.";
  $sql =<<<EOSQL
select count(*) from node n join
field_revision_field_subscribers fs on fs.entity_id = nid
where uid = :uid and title = :title
EOSQL;
  $result = db_query($sql, array('title' => $item_template->title, 'uid' => $uid));
  $count = $result->fetchField();
  if($count) return "An item with this title is listed in your account";
  $auth_code = price_check_get_ebay_auth_code($ebay_account);
  if($auth_code) {
    $item_id_response = price_check_ebay_list_item($item_template, $auth_code);
    $item_id = @$item_id_response['ItemID'];
    if($item_id) {price_check_add_subscriber($item_template, "$ebay_account|$item_id");
      price_check_remove_suggested_account($item_template, $ebay_account);
      node_save($item_template);
      $output = "<h2>Item listed successfully</h2><a href='" . price_check_get_ebay_item_url($item_id) . 
          "'>" . $item_template->title . "</a>";
      return $output;
    } else {
      $edit_link = "<a href='/node/$nid/edit?rawform=1'>edit this suggestion</a>";
      return price_check_translate_xml_errors($item_id_response['Errors']) . $edit_link;
    }
  }
  return "You need to log in to Ebay.";
}

function price_check_get_seller_items_form($form, &$form_state) {
  $user_full = price_check_get_current_user();
  if(!@$user_full->field_ebay_auth_token['und'][0]['value']) {
    drupal_set_message("Contact support to get your ebay API set up");
    return;  
  }
  $batch = @$_SESSION['batch_2'][0];
  if($batch) {
    batch_set($batch);
    unset($_SESSION['batch_2']);
    batch_process('get_seller_items');
  } else {
    $form['seller'] = array(
      '#type' => 'textfield', 
      '#title' => 'Seller',
      '#size' => 80,
      '#maxlength' => 255,
      '#required' => TRUE, 
    );
    $form['min_sales'] = array(
      '#type' => 'textfield', 
      '#title' => 'Min Quantity Sold',
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => TRUE, 
    );
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
    if(@$_SESSION['get_seller_items_output']) {
      $count = count($_SESSION['get_seller_items_output']);
      if($count) {
        $output = "<h3>$count items found</h3><table><tr><th>Title</th></tr>";
        $items = $_SESSION['get_seller_items_output'];
        foreach($items as $item) {
          $title = $item['title'];
          $output .= "<tr><td>$title</td></tr>";
        }
        $output .= "</table>";
      } else {
        $output = "No results found";
      }
      $form['item_output'] = array(
        '#type' => 'item', 
        '#title' => 'Results',
        '#markup' => $output,
      );    
      unset($_SESSION['get_seller_items_output']);
    }
    return $form;  
  }
}

function price_check_get_seller_items_form_submit($form, &$form_state) {
  $seller = $form_state['values']['seller'];
  $sellers = price_check_get_blacklist(true);
  if(in_array(strtolower($seller), $sellers)) return "can't run seller";
  $min_sales = $form_state['values']['min_sales'];
  $page = 1;
  $operations = array();
  $data = price_check_get_ebay_seller_items($seller, $page);
  //print_r($data); exit(0);
  $items = price_check_force_json_array($data['searchResult']['item']);
  $pages = $data['paginationOutput']['totalPages'];
  $results = $data['paginationOutput']['totalEntries'];
  if($results == 0) return "No results";
  if($pages > 1) {
    for($i=2;$i<=$pages;$i++) {
      $operations[] = array(
      'price_check_batch_add_seller_items_request',
        array(
          $seller,
          $i,
          $min_sales,
        ),
      );
    }
  }
  $_SESSION['seller_items'] = array();
  foreach($items as $item) {
    //if(price_check_get_item_sales_30($item['itemId']) >= $min_sales) {
    $_SESSION['seller_items'][] = array('item' => $item['itemId'], 'url' => $item['viewItemURL'], 'min_sales' => $min_sales);
  }
  $batch = array(
    'title' => 'Step 1: Getting items from seller ' . $seller,
    'operations' => $operations,
    'finished' => 'price_check_seller_items_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('get_seller_items');
}

function price_check_batch_add_seller_items_request($seller, $page, $min_sales, &$context) {
  $data = price_check_get_ebay_seller_items($seller, $page);
  $items = price_check_force_json_array($data['searchResult']['item']);
  $results = $data['paginationOutput']['totalEntries'];
  if(!$results) return;
  foreach($items as $item) {
    $_SESSION['seller_items'][] = array('item' => $item['itemId'], 'url' => $item['viewItemURL'], 'min_sales' => $min_sales);
  }
}

function price_check_seller_items_finished($success, $results, $operations)  {
  $items = $_SESSION['seller_items'];
  $operations = array();
  foreach($items as $item) {
    $operations[] = array(
      'price_check_batch_get_seller_items_details_atom',
      array(
        $item,
      ),
    );
  }
  $batch = array(
    'title' => 'Step 2: Deep Scan and Save Results',
    'operations' => $operations,
    'finished' => 'price_check_seller_items_search_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  
  unset($_SESSION['seller_items']);
  $_SESSION['batch_2'] = array($batch);
  return;
}

function price_check_seller_items_search_finished($success, $results, $operations)  {
  $user_full = price_check_get_current_user();
  $nids = $results;
  $nodes = node_load_multiple($nids);
  foreach($nodes as $node) {
    $_SESSION['get_seller_items_output'][] = array('title' => $node->title, 'nid' => $node->nid);
  }
  if (in_array('Paying Customer', $user_full->roles)) {
    return;
  }
  $active_workers = price_check_get_active_workers();
  if($worker_count = count($active_workers)) {
    $workload = count($results) / $worker_count;
    $partitions = price_check_partition($results, $worker_count);
    foreach($active_workers as $i => $worker) {
      price_check_assign_work($worker, @$partitions[$i]);
    }
  }
}

function price_check_assign_work($worker, $partition) {
  //use a queue
  $queue = DrupalQueue::get('price_check_assign_work_queue_atom'); 
  $queue->createQueue();
  foreach($partition as $item)
    $queue->createItem(array('worker' => $worker, 'item' => $item));   
}

function price_check_get_active_workers() {
  $sql = "select entity_id from field_data_field_active_worker where field_active_worker_value = 1";
  $results = db_query($sql);
  $output = array();
  foreach($results as $row) {
    $output[] = $row->entity_id;
  }
  return $output;
}

function price_check_batch_get_seller_items_details_atom($item, &$context) {
  $limit = @$_SESSION['price_check_get_seller_items_multiple_settings']['limit_results'];
  if($limit && count($context['results']) >= $limit) {
    $context['finished'] = 1;
    return;
  }
  global $user;
  $ebay_url = $item['url'];
  //did we import this before?
  if(price_check_is_paying_customer()) {
    $sql = <<<SQL
select count(*) n from field_data_field_ebay_url fu
join field_data_field_imported fi on fu.entity_id = fi.entity_id
join node on nid = fu.entity_id
where field_ebay_url_value = :ebay_url and
uid = :uid
SQL;
    $result = db_query($sql, array(':ebay_url' => $ebay_url, ':uid' => $user->uid));
    $n = $result->fetchField();  
    if($n > 0) {
      $context['message'] = "Skipping duplicate item {$item['title']}";
      return; 
    }
  } else {
    $sql = <<<SQL
select count(*) n from field_data_field_ebay_url fu
join field_data_field_imported fi on fu.entity_id = fi.entity_id
where field_ebay_url_value = :ebay_url
SQL;
    $result = db_query($sql, array(':ebay_url' => $ebay_url));
    $n = $result->fetchField();  
    if($n > 0) {
      $context['message'] = "Skipping duplicate item {$item['title']}";
      return; 
    }
  }  
  if(price_check_get_item_sales_30($item['item']) < $item['min_sales']) {
    $context['message'] = "Skipping item {$item['title']} because too few sales";
    return;
  }
  $item = price_check_get_ebay_item_from_url($ebay_url);
  $title = $item['Item']['Title'];
  $description = price_check_strip_tags($item['Item']['Description']);
  $price = $item['Item']['StartPrice'];
  if(!@$item['Item']['ConditionID']) return; //$item['Item']['ConditionID'] = 1000;
  $condition = $item['Item']['ConditionID'];
  $category = $item['Item']['PrimaryCategory']['CategoryID'];
  $seller = $item['Item']['Seller']['UserID'];
  $quantity_sold = price_check_get_item_sales_30($item);
  $images_array = price_check_force_json_array($item['Item']['PictureDetails']['PictureURL']);
  $nid = @price_check_save_item_template_node($ebay_url, $title, $description, $images_array, $response, $price, $condition, $category, $seller, $quantity_sold);
  $context['results'][] = $nid;
  $context['message'] = "Added item $title to your research";
  return;
}

function price_check_get_seller_items_multiple_form($form, &$form_state) {
  $user_full = price_check_get_current_user();
  if(!@$user_full->field_ebay_auth_token['und'][0]['value']) {
    drupal_set_message("Contact support to get your ebay API set up");
    return;  
  }
  $batch = @$_SESSION['price_check_get_seller_items_multiple_batch'][0];
  if($batch) {
    batch_set($batch);
    unset($_SESSION['price_check_get_seller_items_multiple_batch']);
    batch_process('get_seller_items_multiple');
  } else {
    unset($_SESSION['price_check_get_seller_items_multiple_settings']);
    $payload = @$_GET['payload'];
    if($payload) {
      $sellers = unserialize(urldecode($payload));
      $sellers_list = implode("\r\n", $sellers);
      $form['seller'] = array(
        '#type' => 'textarea', 
        '#title' => 'Seller',
        '#size' => 80,
        '#rows' => 20,
        '#required' => TRUE, 
        '#default_value' => $sellers_list,
      );
    } else {
      $form['seller'] = array(
        '#type' => 'textarea', 
        '#title' => 'Seller',
        '#size' => 80,
        '#rows' => 20,
        '#required' => TRUE, 
      );
    }
    $form['min_sales'] = array(
      '#type' => 'textfield', 
      '#title' => 'Min Quantity Sold',
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => TRUE, 
    );
    $form['min_price'] = array(
      '#type' => 'textfield', 
      '#title' => 'Min Selling Price',
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => FALSE, 
    );
    $form['limit_results'] = array(
      '#type' => 'textfield', 
      '#title' => 'Limit Results',
      '#size' => 6,
      '#maxlength' => 6,
      '#required' => FALSE, 
      '#description' => 'To preserve API calls, enter how many items you want to show up in your research. ' .
        'DsGenie will stop searching for items when this number is reached. 10 is a good number if you are not sure.',
    );
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
    if(@$_SESSION['get_seller_items_multiple_output']) {
      $count = count($_SESSION['get_seller_items_multiple_output']);
      if($count) {
        $output = "<h3>$count items found</h3><table><tr><th>Title</th></tr>";
        $items = $_SESSION['get_seller_items_multiple_output'];
        foreach($items as $item) {
          $title = $item['title'];
          $output .= "<tr><td>$title</td></tr>";
        }
        $output .= "</table>";
      } else {
        $output = "No results found";
      }
      $form['item_output'] = array(
        '#type' => 'item', 
        '#title' => 'Results',
        '#markup' => $output,
      );    
      unset($_SESSION['get_seller_items_multiple_output']);
    }
    return $form;  
  }
}

function price_check_get_seller_items_multiple_form_submit($form, &$form_state) {
  $limit = @$form_state['values']['limit_results'];
  if($limit) $_SESSION['price_check_get_seller_items_multiple_settings']['limit_results'] = $limit;
  $sellers_raw = $form_state['values']['seller'];
  $min_sales = $form_state['values']['min_sales'];
  $min_price = str_replace('$', '', @$form_state['values']['min_price']) + 0;
  if(!$min_price) $min_price = 0;
  $page = 1;
  $operations = array();
  $sellers = explode("\n", $sellers_raw);
  $sellers_blacklist = price_check_get_blacklist(true);
  foreach($sellers as $seller) {
    if(in_array(strtolower($seller), $sellers_blacklist)) continue;
    $operations[] = array(
      'price_check_batch_add_seller_items_multiple_request',
      array(
        $seller,
        $page,
        $min_sales,
        $min_price,
      ),
    );
  }
  $batch = array(
    'title' => 'Step 1: Getting items from sellers ',
    'operations' => $operations,
    'finished' => 'price_check_seller_items_multiple_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('get_seller_items_multiple');
}

function price_check_get_seller_items_multiple_form_validate($form, &$form_state) {
  $sellers = $form_state['values']['seller'];
  $cost = count($sellers) * 400;
  $usage = price_check_get_api_usage();
  $balance = $usage - 4800;
  $possible_sellers = floor($balance / 400);
  if($cost + $usage > 4800) {
    form_set_error('seller', "You do not have enough API credits to run these sellers. Try running less sellers ($possible_sellers)");
  }
}

function price_check_batch_add_seller_items_multiple_request($seller, $page, $min_sales, $min_price, &$context) {
  $data = price_check_get_ebay_seller_items($seller, $page);
  $items = price_check_force_json_array($data['searchResult']['item']);
  $results = $data['paginationOutput']['totalEntries'];
  if(!$results) {
    $context['message'] = "No results for $seller";
    return;
  }
  $items_found = 0;
  foreach($items as $item) {
    if($item['isMultiVariationListing'] == 'true') continue;
    if($item['autoPay'] == 'false') continue;
    if($item['sellingStatus']['currentPrice'] < $min_price) continue;
    $items_found++;
    $context['results'][] = array('item' => $item['itemId'], 'url' => $item['viewItemURL'], 'min_sales' => $min_sales, 'title' => $item['title']);
  }
  $context['message'] = "Added $items_found for deep scan from $seller";
}

function price_check_seller_items_multiple_finished($success, $results, $operations)  {
  $items = $results;
  $operations = array();
  foreach($items as $item) {
    $operations[] = array(
      'price_check_batch_get_seller_items_details_atom',
      array(
        $item,
      ),
    );
  }
  $batch = array(
    'title' => 'Step 2: Deep Scan and Save Results',
    'operations' => $operations,
    'finished' => 'price_check_seller_items_search_multiple_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  $_SESSION['price_check_get_seller_items_multiple_batch'] = array($batch);
  return;
}

function price_check_seller_items_search_multiple_finished($success, $results, $operations)  {
  $user_full = price_check_get_current_user();
  $nids = $results;
  $nodes = node_load_multiple($nids);
  foreach($nodes as $node) {
    $_SESSION['get_seller_items_multiple_output'][] = array('title' => $node->title, 'nid' => $node->nid);
  }
  if (in_array('Paying Customer', $user_full->roles)) {
    return;
  }
  $active_workers = price_check_get_active_workers();
  if($worker_count = count($active_workers)) {
    $workload = count($results) / $worker_count;
    $partitions = price_check_partition($results, $worker_count);
    foreach($active_workers as $i => $worker) {
      price_check_assign_work($worker, @$partitions[$i]);
    }
  }
}

function price_check_get_orders_form($form, &$form_state) {
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  if(@$_SESSION['proc_get_orders_output']['order_total']) {
    $count = count($_SESSION['proc_get_orders_output']['order_total']);
    if($count) {
      $output = implode(' orders <br />', $_SESSION['proc_get_orders_output']['order_total']);
    } else {
      $output = "No results found";
    }
    unset($_SESSION['proc_get_orders_output']);
    $form['item_output'] = array(
      '#type' => 'item', 
      '#title' => 'Results',
      '#markup' => $output,
    );
    return $form;
  } 
  if(@$_SESSION['proc_get_orders_output']) {
    //prepare second batch 
    $operations = $_SESSION['proc_get_orders_output'];
    if(!$operations) {
      $output = "You need to log in to at least one Ebay Account.";
      $form['item_output'] = array(
        '#type' => 'item', 
        '#title' => 'Results',
        '#markup' => $output,
      );
      return $form;
    }
    $batch = array(
      'title' => 'Getting Ebay Order Details',
      'operations' => $operations,
      'finished' => 'price_check_save_node_orders_atom_finished',
      'file' => 'pages.inc',
    );
    batch_set($batch);
    batch_process('get_orders');
    return;
  }
  return $form;  
}

function price_check_get_orders_form_submit($form, &$form_state) {
  $auth_tokens = price_check_get_ebay_auth_tokens();
  $operations = array();
  foreach($auth_tokens as $auth_token) {
    if($auth_token == '-') continue;
    $operations[] = array(
      'price_check_save_node_orders',
      array($auth_token),
    );
  }
  if(!$operations) return "You need to log in to at least one Ebay Account.";
  $batch = array(
    'title' => 'Getting Ebay Orders',
    'operations' => $operations,
    'finished' => 'price_check_save_node_orders_finished',
    'file' => 'pages.inc',
  );
  batch_set($batch);
  batch_process('get_orders');   
}

function price_check_save_node_orders($auth_token, &$context) {
  $data = price_check_get_ebay_orders($auth_token);
  $pages = $data['PaginationResult']['TotalNumberOfPages'];
  if($data['PaginationResult']['TotalNumberOfEntries'] == 0) {
    $_SESSION['proc_get_orders_output']['order_total'] = "You have no orders at this time.";
    return;
  }
  $pages = $data['PaginationResult']['TotalNumberOfPages'];
  $operations = array();
  for($i=1;$i<=$pages;$i++) {
    $operations[] = array(
      'price_check_save_node_orders_atom',
      array($auth_token, $i),
    );
  }
  $context['results'] = $operations;
  //$_SESSION['proc_get_orders_output'][] = "$order_total new orders for $ebay_account";  
}

function price_check_save_node_orders_finished($success, $results, $operations) {
  //print_r($results); exit(0);
  $_SESSION['proc_get_orders_output'] = $results;
}

function price_check_save_node_orders_atom($auth_token, $page, &$context) {
  $data = price_check_get_ebay_orders($auth_token, $page);
  $current_page = $data['PageNumber'];
  if($data['PaginationResult']['TotalNumberOfEntries'] == 1) {
    $orders = array(0 => $data['TransactionArray']['Transaction']);
  } else {
    $orders = $data['TransactionArray']['Transaction'];
  }
  if(@$orders['Buyer']) $orders = array(0 => $data['TransactionArray']['Transaction']);
  $ebay_account = $data['Seller']['UserID'];
  $output = array();
  $nums = array();
  foreach($orders as $order) {
    $assoc_order = array( 
      "Title" => @$order['ShippingDetails']['SellingManagerSalesRecordNumber'],
      "Ebay Account" => $ebay_account,	
      "Ebay Buyer ID" => @$order['Buyer']['UserID'],
      "Ebay Order ID" => @$order['OrderLineItemID'],        
      "Purchase Date" => @$order['PaidTime'], 
      "Item name" => price_check_get_item_name($order['Item']['ItemID']),
      "Customer Price" => @$order['AmountPaid'],
      "Quantity" => @$order['QuantityPurchased'],
      "Customer Name" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['Name'],
      "Shipping Address 1" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['Street1'],
      "Shipping Address 2" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['Street2'],
      "Shipping City" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['CityName'],
      "Shipping Zip" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['PostalCode'],// 18704-1976
      "Shipping Country" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['CountryName'],
      "Shipping State" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['StateOrProvince'],
      "Customer Tel" => @$order['Buyer']['BuyerInfo']['ShippingAddress']['Phone'],
      "Ebay Item ID" => @$order['Item']['ItemID'],
      "Ebay Buyer Comment" => @$order['BuyerCheckoutMessage'],
    );
    $output[$ebay_account][$order['ShippingDetails']['SellingManagerSalesRecordNumber']] = $assoc_order;
  }
  $order_total = price_check_save_new_orders($output);
  $context['results'][] = $order_total;
}

function price_check_save_node_orders_atom_finished($success, $results, $operations) {
  $_SESSION['proc_get_orders_output']['order_total'] = array(array_sum($results));
}

/*
function price_check_customers() {
  //get listing count
  $sql = <<<SQL
select count(*) num_listings from field_data_field_complete fc 
join field_data_field_imported fi on fi.entity_id = fc.entity_id 
join field_data_field_store_price fp on fi.entity_id = fp.entity_id 
join node on node.nid = fc.entity_id
where field_complete_value = 1 and 
uid in (5,1,2290)
SQL;
  $result = db_query($sql);
  $num_listings = $result->fetchField();
  $role = user_role_load_by_name("Paying Customer");
  $query = 'SELECT ur.uid
    FROM {users_roles} AS ur
    WHERE ur.rid = :rid';
  $result = db_query($query, array(':rid' => $role->rid));
  $uids = $result->fetchCol();
  $users = user_load_multiple($uids);
  $output = "<h3>$num_listings available</h3><table><tr><th>Name</th><th>User Name</th><th>Ebay Account</th><th>Subscription Ends</th><th>Has API?</th><th>Give Listings</th></tr>";
  foreach($users as $account) {
    if(@$account->field_ebay_auth_token['und'][0]['value']) {
      $has_api = "YES";
    } else {
      $has_api = "NO";
    }
    $stripe_id = @$account->field_stripe_id['und'][0]['value'];
    $uid = $account->uid;
    if($stripe_id) {
      price_check_call_stripe();
      $customer = \Stripe\Customer::retrieve($stripe_id);
      $subscription_id = @$account->field_subscription_id['und'][0]['value'];
      if($subscription_id) {
        if(@!$customer->subscriptions) {
          print_r($subscription_id); exit(0);
        }
        $subscription = $customer->subscriptions->retrieve($account->field_subscription_id['und'][0]['value']);
        $status = $subscription->status;
        $end_timestamp = $subscription->current_period_end;
        $end_date = date('Y-m-d', $end_timestamp);
      } else $end_date = "FREE/DELINQUENT";
    } else
      $end_date = "FREE";
    $points_link = '';
    foreach(array(1,3,5,10) as $points) {
      $points_link .= "<a href='/give_listings/$uid/$points'>+$points</a> ";
    }
    $output .= "<tr><td>{$account->field_user_full_name['und'][0]['value']}</td>" .
      "<td>{$account->name}</td>" .
      "<td>" . implode(',', price_check_decode_field($account->field_ebay_accounts['und'])) . "</td>" .
      "<td>$end_date</td>" .
      "<td>$has_api</td>" .    
      "<td>$points_link</td>" .
      "</tr>";  
  }
  $output .= "</table>";
  return $output;
}
 * 
 */

function price_check_customers() {
  //get listing count
  $sql = <<<SQL
select count(*) num_listings from field_data_field_complete fc 
join field_data_field_imported fi on fi.entity_id = fc.entity_id 
join field_data_field_store_price fp on fi.entity_id = fp.entity_id 
join node on node.nid = fc.entity_id
where field_complete_value = 1 and 
uid in (5,1,2290)
SQL;
  $result = db_query($sql);
  $num_listings = $result->fetchField();
  $header = array(
    'Name' => array(
      'data' => 'Name',
      'type' => 'property',
      'specifier' => 'name',
    ),
    'User Name',
    'Ebay Account',
    'Anniversary Date',
    'Last Payment',
    'Paypal Email',
    'Has API?',
    'Give Listings',
  );
  $rows = array();

  if(true) {
    $count = 20;
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'user')
      ->propertyCondition('uid', 0, '!=')
      ->addTag('role_join');
    $query = $query->pager($count)
      ->tableSort($header);   
    $results = $query->execute();    
    if(!@$results['user']) return "no items listed";
    foreach($results['user'] as $uid => $tmp) {
      $account = user_load($uid);
      if(@$account->field_ebay_auth_token['und'][0]['value']) {
        $has_api = "YES";
      } else {
        $has_api = "NO";
      }
      $points_link = '';
      foreach(array(1,3,5,10) as $points) {
        $points_link .= "<a href='/give_listings/$uid/$points'>+$points</a> ";
      }
      $anniv_date = @$account->field_anniversary_date['und'][0]['value'];
      $last_paid_date = @$account->field_last_paid['und'][0]['value'];
      $paypal_email = @$account->field_paypal_email['und'][0]['value'];
      $edit_link = "<a href='/user/$uid/edit'>{$account->name}</a>";
      $rows[] = array(
        $account->field_user_full_name['und'][0]['value'],
        $edit_link,
        implode(',', price_check_decode_field($account->field_ebay_accounts['und'])),
        $anniv_date,
        $last_paid_date,
        $paypal_email,
        $has_api,   
        $points_link,
      );  
    }
    $render_array['num_listings'] = array(
      '#type' => 'markup',
      '#markup' => "<h3>$num_listings available</h3>",
    );
    $render_array['table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#empty' => 'Empty List - No Entries',
      '#header' => $header,
    );
    $render_array['pager'] = array(
      '#theme' => 'pager',
    );
    return $render_array;
  }
}

function price_check_ebay_listings() {
  $item_search = @$_POST['item_search'];
  if(@$_GET['count']) {
    $count = $_GET['count'];
  } else $count = 20;
  $header = array('Image',
    'Item Name' => array(
      'data' => 'Item Name',
      'type' => 'property',
      'specifier' => 'title',
    ),
    'Price' => array(
      'data' => 'Price',
      'type' => 'field',
      'specifier' => array('field' => 'field_price', 'column' => 'value'),
    ),
    'Set Price',
    'Set Qty',
    'Action',
    'S/G Status'=> array(
      'data' => 'S/G Status',
      'type' => 'field',
      'specifier' => array('field' => 'field_skugrid_local_id', 'column' => 'value'),
    ),
    'Remove',
  );
  $rows = array();
  global $user;
  $uid = $user->uid;
  if($uid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'item_template') 
      ->propertyCondition('uid', $uid)
      ->fieldCondition('field_subscribers', 'value', '', '>')
      ->propertyCondition('title', "%".$item_search."%","LIKE") 
      ->pager($count)
      ->tableSort($header);    
    $results = $query->execute();    
    if(!@$results['node']) return "no items listed";
    foreach($results['node'] as $nid => $tmp) {
      $record = node_load($nid);
      $params = $record->field_subscribers['und'][0]['value'];
      $subscriber = price_check_get_subscriber_from_string($params);
      $price_html = '';
      if($subscriber['price']) $price_html = " ({$subscriber['price']})";
      $ebay_id = $subscriber['ebay_id'];
      $item = node_load($nid);
      if(!$item) continue;
      $edit_link = "<a href='/edit_ebay/$ebay_id'>(edit)</a>";
      $ext_link = "<a target='_blank' href='" . htmlspecialchars(@$item->field_external_url['und'][0]['value']) . "'>(link)</a>";
      $pictures = price_check_decode_field(@$item->field_picture['und']);
      if(count($pictures) > 0) {
        $picture = $pictures[0];
      } else {
        $pictures = price_check_decode_picture_field(@$item->field_ds_genie_pictures['und']);
        if(count($pictures) > 0) $picture = $pictures[0];
      }
      if(@$picture) {
        $img = "<img src='$picture' width='100' />";
      } else 
        $img = "N/A";
      $price = @$item->field_price['und'][0]['value'];
      $local_id = @$item->field_skugrid_local_id['und'][0]['value'];
      if($local_id) {
        $skugrid_info = "#$local_id";
      } else {
        //if(!@$_GET['debug']) {
          //$skugrid_info = "<a href='#' onclick='javascript:alert(\"stand by, in development.\");'>convert</a>";
        //} else {
        $skugrid_info = "<a href='#' onclick=\"javascript:jQuery(this.parentElement).html('<img src='/load.gif' />').load('/convert_skugrid/$nid');\">convert</a>";
        $skugrid_info = "<a href='#' onclick=\"javascript:jQuery(this.parentElement).html('<img src=/load.gif />').load('/convert_skugrid/$nid'); return false;\">convert</a>";
        //}
      }
      
      //$obj->upc = 
  
      
      $remove_link = "<a href='/remove_item/$nid/$ebay_id' onclick='return confirm(\"Are you sure?\")'>CANCEL LISTING</a>";
      $rows[] = array(
        $img,
        "<a href='" . price_check_get_ebay_item_url($ebay_id)
        . "'>". @$item->title . "</a> $ext_link $edit_link",
        $price,
        "<input type='text' size='4' name='p$nid' />",
        "<input type='text' size='4' name='q$nid' />",
        "<input type='hidden' name='x$nid' value='$params' /><input type='submit' value='Submit'>",
        $skugrid_info,
        $remove_link,
      );  
    }
    $search_html = <<<EOHTML
<form id='modify_item_search' method='POST'>
<input type="text" id="item_search" name="item_search" value="" size="90" maxlength="80" class="form-text">
<input type="submit" id="item_search_submit" name="item_search_submit" value="Search" class="form-submit">
</form>        
EOHTML;
    $render_array['form-search'] = array(
      '#type' => 'markup',
      '#markup' => $search_html,
    );
    $render_array['form-markup'] = array(
      '#type' => 'markup',
      '#markup' => "<form id='modify_item' method='POST' action='/modify_item_ajax'>",
    );
    $render_array['table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#empty' => 'Empty List - No Entries',
      '#header' => $header,
    );

    $render_array['pager'] = array(
      '#theme' => 'pager',
    );
    
    $render_array['form-markup-post'] = array(
      '#type' => 'markup',
      '#markup' => "</form>",
    );

    return $render_array;

  }
  return "you have no listings";
}

function price_check_my_sellers_form($form, &$form_state) {
  $node = price_check_get_bot();
  $sellers = price_check_decode_field(@$node->field_sellers['und'], true);
  if($sellers) {
    $seller_count = count($sellers);
    $form['markup'] = array(
      '#type' => 'item',
      '#markup' => "<h3>$seller_count saved sellers</h3>",
    );
    $form['seller'] = array(
      '#type' => 'textfield', 
      '#title' => 'Add Seller',
      '#description' => 'Enter the seller name as you would in the eBay seller search box',
    );
    $form['submit_button_seller'] = array(
      '#type' => 'submit',
      '#value' => t('Add Seller'),
    );
    $form['sellers'] = array(
      '#type' => 'checkboxes',
      '#options' => drupal_map_assoc($sellers),
      '#title' => t('My Sellers'),
    );
    $form['submit_button_run'] = array(
      '#type' => 'submit',
      '#value' => t('Run Selected'),
    );
    $form['submit_button_delete'] = array(
      '#type' => 'submit',
      '#attributes' => array('onclick' => 'if(confirm("Are you sure?")) return true; return false;'),
      '#value' => t('Delete Selected'),
    );
  } else {
    $form['markup'] = array(
      '#type' => 'item',
      '#markup' => 'You have no sellers saved.',
    );
  }
  return $form;
}

function price_check_my_sellers_form_submit($form, &$form_state) {
  $node = price_check_get_bot();
  if($form_state['values']['op'] == 'Add Seller') {
    $seller = @$form_state['values']['seller'];
    if($seller) {
      $saved_sellers = price_check_decode_field(@$node->field_sellers['und'], true);
      if(!in_array($seller, $saved_sellers)) {
        $saved_sellers[] = $seller;
        $node->field_sellers['und'] = price_check_encode_field($saved_sellers);
        node_save($node);
        drupal_set_message("Added $seller to your sellers.");
        return;
      } else {
        drupal_set_message("$seller is already in your sellers.");
        return;
      }
    }
  } 
  if($form_state['values']['op'] == 'Delete Selected') {
    $sellers = @$form_state['values']['sellers'];
    $saved_sellers = price_check_decode_field(@$node->field_sellers['und'], true);
    $output = array();
    foreach($saved_sellers as $seller) {
      if(!@$sellers[$seller]) {
        $output[] = $seller;
      }
    }
    $node->field_sellers['und'] = price_check_encode_field($output);
    node_save($node);   
  }
  if($form_state['values']['op'] == 'Run Selected') {
    $sellers = @$form_state['values']['sellers'];
    $output = array();
    foreach($sellers as $seller) {
      if($seller) $output[] = $seller;
    }
    if($output) {
      $arg = urlencode(serialize($output));
      header("Location: /get_seller_items_multiple?payload=$arg");
      exit();
    }
  }
}
  

function price_check_search_title_form($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  if(@$_SESSION['search_title_output']) {
    $sellers = explode(',', $_SESSION['search_title_output'][0]);
    $count = count($sellers);
    $form['sellers'] = array(
      '#title' => 'Sellers Found',
      //'#rows' => $count,
      '#type' => 'textarea',
      '#default_value' => $_SESSION['search_title_output'][0],
    );
    /*$form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Save Sellers'),
    );
    /*$form['markup'] = array(
      '#type' => 'item',
      '#markup' => $_SESSION['search_title_output'][0],
    );*/
    unset($_SESSION['search_title_output']);
  } else {
    $form['title'] = array(
      '#type' => 'textfield', 
      '#title' => 'Ebay Title',
      '#size' => 80,
      '#maxlength' => 80,
      '#required' => TRUE,
    );
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
  }
  return $form; 
}

function price_check_search_title_form_submit($form, &$form_state) {
  $title = @$form_state['values']['title'];
  $results = price_check_get_items_by_keywords($title);
  $items = @$results['searchResult']['item'];
  if(@$items) {
    if(@$items['itemId']) $items = array($items);
    $unsaved_sellers = array();
    $sellers = price_check_get_blacklist();
    foreach($items as $item) {
      $seller = $item['sellerInfo']['sellerUserName'];
      if(!in_array($seller, $sellers)) $unsaved_sellers[] = $seller;
    }
    //$_SESSION['search_title_output'] = array(implode(", ", $output));
    //save seller list
    $node = price_check_get_bot();
    $saved_sellers = price_check_decode_field(@$node->field_sellers['und'], true);
    $output = array();
    foreach($unsaved_sellers as $raw_seller) {
      if(!in_array(trim($raw_seller), $saved_sellers)) {
        $saved_sellers[] = trim($raw_seller);
        $output[] = trim($raw_seller);
        drupal_set_message("Found seller: $raw_seller");
      }
    }
    $node->field_sellers['und'] = price_check_encode_field($saved_sellers);
    node_save($node);
    $count = count($output);
    drupal_set_message("$count sellers added to list");
    //drupal_goto('/');
  } else {
    drupal_set_message("No results found for your title.");
  }
}

function price_check_ebay_listings_user_report($success = 'not started', $results = array(), $operations = array())   {
  $ebay_accounts = price_check_get_ebay_account_names();
  $sql_args = price_check_build_sql_accounts($ebay_accounts);

  $sql = "select * from field_data_field_subscribers where " . $sql_args['sql'];
  $result = db_query($sql, $sql_args['args']);
  $operations = array();
  foreach($result as $record) {
    $operations[] = array('price_check_ebay_listings_user_report_atom',
      array($record));
  }
  $batch = array(
    'title' => 'Loading your Ebay Items (only those tracked by DS Genie)',
    'operations' => $operations,
    'finished' => 'price_check_ebay_listings_user_report_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('listings_user_report_complete');
}

function price_check_ebay_listings_user_report_atom($record, &$context) {
  $nid = $record->entity_id;
  $params = $record->field_subscribers_value;
  $subscriber = price_check_get_subscriber_from_string($params);
  $ebay_id = $subscriber['ebay_id'];
  $item = node_load($nid);
  if(!$item) return;
  $pictures = price_check_decode_field(@$item->field_picture['und']);
  if(count($pictures) > 0) {
    $picture = $pictures[0];
  } else {
    $pictures = price_check_decode_picture_field(@$item->field_ds_genie_pictures['und']);
    if(count($pictures) > 0) $picture = $pictures[0];
  }
  if(@$picture) {
    $img = "<img src='$picture' width='100' />";
  } else 
    $img = "N/A";
  //$sugg_price = @$item->field_price['und'][0]['value'];
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  $ebay_price = $ebay_item['Item']['StartPrice'];
  $item->field_price['und'][0]['value'] = $ebay_price;
  node_save($item);
  $src_price = $item->field_store_price['und'][0]['value'];
  $markup = number_format(($ebay_price - $src_price) * 100 / $src_price, 2);
  $profit = number_format(($ebay_price  - ($ebay_price * 0.029)  - 0.3 - ($ebay_price * 0.09) - $src_price) * 100 / $src_price, 2);
  if(!$ebay_item) return;

  $output = "<tr><td>$img</td>" .
      "<td><a href='" . price_check_get_ebay_item_url($ebay_id) . "'>". @$item->title . "</a></td>" .
      //"<td>$sugg_price</td>" .
      "<td>$ebay_price</td>" .
      "<td>$src_price</td>" .
      "<td>$markup%</td>" .
      "<td>$profit%</td>" .
      "<td><a href='/node/$nid/edit?rawform=1&check_ebay=1'>edit</a></td></tr>";
  $context['results'][] = $output;
}

function price_check_ebay_listings_user_report_finished($success, $results, $operations)   {
  $output = "<p>You have no listings at this time.</p>";
  if($results) {
    $output = "<table><tr><th>Image</th><th>Item Name</th><th>Ebay Price</th><th>Src Price</th><th>Markup</th><th>Profit</th><th>edit</th></tr>";
    foreach($results as $result) {
      $output .= $result;
    }
    $output .= "</table>";
  }
  $_SESSION['price_check_ebay_listings_user_report'] = array($output);
}
function price_check_ebay_listings_user_report_complete() {
  return implode('', $_SESSION['price_check_ebay_listings_user_report']);
}

function price_check_seller_work($seller) {
  global $user;
  if(!@$seller) return;
  $user_full = price_check_get_current_user();
  if (in_array('Paying Customer', $user_full->roles)) {
    drupal_set_title("Research by Seller");
    $sql = <<<SQL
select title, nid, field_price_value, field_quantity_sold_value from field_data_field_seller fs 
join node n on n.nid = fs.entity_id
join field_data_field_price fp on fp.entity_id = fs.entity_id
join field_data_field_quantity_sold fq on fq.entity_id = fs.entity_id
where uid = :uid and
field_seller_value = :seller and 
nid not in (select entity_id from field_data_field_complete_date)
SQL;
    $results = db_query($sql, array(':uid' => $user->uid, ':seller' => $seller));
    $delete_link = "<a href='/delete_seller/$seller' onclick='return confirm(\"Are you sure?\")'>delete seller</a>";
    $output = "$delete_link<table><tr><th>Item</th><th>Qty Sold</th><th>Price</th><th>Action</th></tr>";
    foreach($results as $row) {
      $nid = $row->nid;
      $edit_link = "<a href='/node/$nid/edit?destination=node/$nid/edit'>edit</a>";
      //$output .= "<tr><td>$row->title</td><td>$edit_link</td></tr>";
      $output .= "<tr><td>{$row->title}</td><td>{$row->field_quantity_sold_value}</td><td>{$row->field_price_value}</td><td>$edit_link</td></tr>";
    }
    $output .= "</table>";
    return $output;
  }
  $sql = <<<SQL
select title, nid, field_price_value from field_data_field_assigned_to fa 
join field_data_field_seller fs on fs.entity_id = fa.entity_id
join node n on n.nid = fs.entity_id
join field_data_field_price fp on fp.entity_id = fs.entity_id
where field_assigned_to_value = :uid and
field_seller_value = :seller and 
nid not in (select entity_id from field_data_field_complete_date where field_complete_date_value > '')
SQL;
  
  $results = db_query($sql, array(':uid' => $user->uid, ':seller' => $seller));
  $delete_link = "<a href='/delete_seller/$seller' onclick='return confirm(\"Are you sure?\")'>delete seller</a>";
  $output = "$delete_link<table><tr><th>Item</th><th>Price</th><th>Action</th></tr>";
  foreach($results as $row) {
    $nid = $row->nid;
    $edit_link = "<a href='/node/$nid/edit?destination=node/$nid/edit'>edit</a>";
    $output .= "<tr><td>{$row->title}</td><td>{$row->field_price_value}</td><td>$edit_link</td></tr>";
  }
  $output .= "</table>";
  return $output;
}

function price_check_my_research() {
  global $user;
  $user_full = price_check_get_current_user();
  if (in_array('Paying Customer', $user_full->roles)) {
    $output = "<table><tr><th>Seller</th><th>Items</th></tr>";
  $sql = <<<SQL
select field_seller_value, count(*) records from node fa 
join field_data_field_seller fs on fs.entity_id = fa.nid
where fa.uid = :uid
and fs.entity_id not in (select entity_id from field_data_field_complete)
and fs.entity_id in (select entity_id from field_data_field_imported where field_imported_value = 1)
and fs.entity_id not in (select entity_id from field_data_field_complete where field_complete_value = 1)
group by field_seller_value
SQL;
    $results = db_query($sql, array(':uid' => $user->uid));
    foreach($results as $row) {
      $records = $row->records;
      $seller = $row->field_seller_value;
      $seller_link = "<a href='/seller_work/$seller'>$seller</a>";
      $output .= "<tr><td>$seller_link</td><td>$records</td></tr>";
    }
    $output .= "</table>";
    return $output;
  }
  
  $timezone = date('T');
  $output = "<h3>Completed " . price_check_get_worker_report_24h($user->uid) . " as of 12AM today ($timezone)</h3>" .
      "<table><tr><th>Seller</th><th>Items</th><th>Value</th></tr>";
  $sql = <<<SQL
select field_seller_value, count(*) records from field_data_field_assigned_to fa 
join field_data_field_seller fs on fs.entity_id = fa.entity_id
join node on node.nid = fa.entity_id
where field_assigned_to_value = :assigned_to
and fs.entity_id not in (select entity_id from field_data_field_complete where field_complete_value = 1) and
nid not in (select entity_id from field_data_field_complete_date where field_complete_date_value > '')
group by field_seller_value
SQL;
  $results = db_query($sql, array(':assigned_to' => $user->uid));
  $total = 0;
  foreach($results as $row) {
    $records = $row->records;
    $total += $records;
    $seller = $row->field_seller_value;
    $seller_link = "<a href='/seller_work/$seller'>$seller</a>";
    $value = number_format((float)$records / 6, 2, '.', '');
    $output .= "<tr><td>$seller_link</td><td>$records</td><td>$value</td></tr>";
  }
  $total_value = number_format((float)$total / 6, 2, '.', '');
  $output .= "<tr><td colspan='2'>Total</td><td>$total_value</td></tr></table>";
  return $output;
}

function price_check_ebay_listings_user_report_a($success = 'not started', $results = array(), $operations = array())   {
  global $user;
  $uid = $user->uid;
  $sql = <<<EOSQL
select fs.entity_id, field_subscribers_value from node n join 
field_data_field_subscribers fs on fs.entity_id = n.nid
join field_data_field_store_price fsp on nid = fsp.entity_id
join field_data_field_price fp on nid = fp.entity_id
where 
(field_price_value > field_store_price_value * 1.3 or
field_price_value < field_store_price_value * 1.15 )
and uid = :uid;
EOSQL;
  $result = db_query($sql, array(":uid" => $uid));
  $operations = array();
  foreach($result as $record) {
    $operations[] = array('price_check_ebay_listings_user_report_atom_a',
      array($record));
  }
  $batch = array(
    'title' => 'Loading your Ebay Items (only those tracked by DS Genie)',
    'operations' => $operations,
    'finished' => 'price_check_ebay_listings_user_report_finished_a',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('listings_user_report_complete');
}

function price_check_ebay_listings_user_report_atom_a($record, &$context) {
  $nid = $record->entity_id;
  watchdog("user report try", $nid);
  $params = $record->field_subscribers_value;
  $subscriber = price_check_get_subscriber_from_string($params);
  $ebay_id = $subscriber['ebay_id'];
  $item = node_load($nid);
  if(!$item) return;
  $pictures = price_check_decode_field(@$item->field_picture['und']);
  if(count($pictures) > 0) {
    $picture = $pictures[0];
  } else {
    $pictures = price_check_decode_picture_field(@$item->field_ds_genie_pictures['und']);
    if(count($pictures) > 0) $picture = $pictures[0];
  }
  if(@$picture) {
    $img = "<img src='$picture' width='100' />";
  } else 
    $img = "N/A";
  //$sugg_price = @$item->field_price['und'][0]['value'];
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  $ebay_price = $ebay_item['Item']['StartPrice'];
  $item->field_price['und'][0]['value'] = $ebay_price;
  node_save($item);
  $src_price = $item->field_store_price['und'][0]['value'];
  $markup = number_format(($ebay_price - $src_price) * 100 / $src_price, 2);
  $profit = number_format(($ebay_price  - ($ebay_price * 0.029)  - 0.3 - ($ebay_price * 0.09) - $src_price) * 100 / $src_price, 2);
  if(!$ebay_item) return;

  $output = "<tr><td>$img</td>" .
      "<td><a href='" . price_check_get_ebay_item_url($ebay_id) . "'>". @$item->title . "</a></td>" .
      "<td>$ebay_price</td>" .
      "<td>$src_price</td>" .
      "<td>$markup%</td>" .
      "<td>$profit%</td>" .
      "<td><a href='/node/$nid/edit?rawform=1&check_ebay=1'>edit</a></td></tr>";
  $context['results'][] = $output;
}

function price_check_ebay_listings_user_report_finished_a($success, $results, $operations)   {
  $output = "<p>You have no listings at this time.</p>";
  if($results) {
    $output = "<table><tr><th>Image</th><th>Item Name</th><th>Ebay Price</th><th>Src Price</th><th>Markup</th><th>Profit</th><th>edit</th></tr>";
    foreach($results as $result) {
      $output .= $result;
    }
    $output .= "</table>";
  }
  $_SESSION['price_check_ebay_listings_user_report'] = array($output);
}

function price_check_display_notifications() {
  $user_full = price_check_get_current_user();
  if(@$_GET['debug']) {
    $sql = "select nid from node where type='item_activity' and status = 1 order by nid desc limit 100";
  } else {
    $sql = "select nid from node where type='item_activity' and status = 1 and uid = :uid";
  }
  $results = db_query($sql, array(":uid" => $user_full->uid));
  $output = "<a id='activity'></a><h2>Item Activity</h2>" .
    "<table><tr><th>Image</th><th>Date</th><th>Item Name</th><th>Message</th><th>Action</th><tr>";
  foreach($results as $row) {
    $item_activity = node_load($row->nid);
    $item_template = node_load($item_activity->field_related_node['und'][0]['value']);
    $message = $item_activity->field_related_message['und'][0]['value'];
    $title = $item_activity->title;
    $pictures = price_check_decode_field(@$item_template->field_picture['und']);
    $picture = @$pictures[0];
    if(!$picture || trim($picture) == '' ) {
      $pictures = price_check_decode_picture_field(@$item_template->field_ds_genie_pictures['und']);
      $picture = @$pictures[0];
    }
    $url = @$item_template->field_external_url['und'][0]['value'];
    if(!$url) continue;
    $subscriber = @price_check_get_subscriber_from_string($item_template->field_subscribers['und'][0]['value']);
    $ebay_id = $subscriber['ebay_id'];
    $ebay_url = price_check_get_ebay_item_url($ebay_id);
    $action_url = '/dismiss-notification/' . $row->nid;
    $date = date("m.d.y H:i:s", $item_activity->created);  
    $output .= "<tr><td><a href='$url' target='_blank'><img src='$picture' width='100' />" .
        "</a></td><td>$date</td><td><a href='$ebay_url' target='_blank'>$title</a></td>" .
        "<td>$message</td><td><a href='$action_url'>dismiss</a></td></tr>";
  }
  $output .= "<tr><td colspan='5'><a href='/dismiss-notification/all' onclick='return confirm(\"Are you sure?\");'>clear activity</a></td></tr>";
  $output .= "</table>";
  return $output;
}

function price_check_replenish_form($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  if(@$_SESSION['rebuild_output']) {
    /*$_SESSION['rebuild_title_output'][0];
    $count = count($sellers);
    $form['sellers'] = array(
      '#title' => 'Sellers Found',
      //'#rows' => $count,
      '#type' => 'textarea',
      '#default_value' => $_SESSION['search_title_output'][0],
    );
    /*$form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Save Sellers'),
    );
    /*$form['markup'] = array(
      '#type' => 'item',
      '#markup' => $_SESSION['search_title_output'][0],
    );*/
    //unset($_SESSION['search_title_output']);*/
  } else {
    $form['default_quantity'] = array(
      '#type' => 'textfield', 
      '#title' => 'Default Quantity',
      '#description' => 'All items zero quantity and are marked in stock will have this quantity added.',
      '#size' => 80,
      '#maxlength' => 80,
      '#required' => TRUE,
    );
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('OK'),
    );
  }
  return $form; 
}

function price_check_replenish_form_submit($form, &$form_state) {
  global $user;
  $quantity = @$form_state['values']['default_quantity'];
  $sql = <<<EOSQL
select title, nid, field_external_url_value, field_subscribers_value from node n 
join field_data_field_store_status fs on fs.entity_id = nid
join field_data_field_subscribers fss on fss.entity_id = nid
join field_revision_field_external_url fu on fu.entity_id = nid
where field_store_status_value = 'Available'
and uid = :uid
EOSQL;
  $uid = $user->uid;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array(); 
  foreach($results as $row) {
    $nid = $row->nid;
    $raw_subscriber = $row->field_subscribers_value;
    $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
    $ebay_id = $subscriber['ebay_id'];
    $title = $row->title;
    $url = $row->field_external_url_value;
    $operations[] = array(
      'price_check_replenish_atom',
      array($uid, $ebay_id, $quantity, $title, $url, $nid),
    );
  }
  $batch = array(
    'title' => 'Replenish eBay Items',
    'operations' => $operations,
    'finished' => 'price_check_replenish_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('/');
}

function price_check_replenish_atom($uid, $ebay_id, $default_quantity, $title, $url, $nid, &$context) {
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id, $uid);
  $quantity = $ebay_item['Item']['Quantity'];
  if($quantity == 0) {
    $auth_tokens = price_check_get_ebay_auth_tokens($uid);
    $auth_code = $auth_tokens[0];
    $result = price_check_set_ebay_item_quantity($ebay_id, $default_quantity, $auth_code);
    if($result['Ack'] == 'Failure') {
      $context['results']['errors'][] = $title;
      $context['message'] = "$title failed update.";
    } else {
      $context['results']['success'][] = $title;
      $context['message'] = "$title updated succesfully.";
      $queue_item = new stdClass();
      $queue_item->entity_id = $nid;
      $queue_item->field_external_url_value = $url; 
      $queue = DrupalQueue::get('price_check_general_queue'); 
      $queue->createQueue();
      $queue->createItem($queue_item); 
    }
  } else {
    $context['message'] = "$title skipped.";
  }
}

function price_check_replenish_finished($success, $results, $operations) {
  $errors = @$results['errors'];
  if($errors) {
    foreach($errors as $error_message) {
      drupal_set_message("$error_message failed", 'warning');
    }
  }
  $success = @$results['success'];
  if($success) {
    foreach($success as $ack) {
      drupal_set_message("$ack updated");
    }
  }
}

function price_check_remove_item($nid, $ebay_id) {
  global $user;
  $node = node_load($nid);
  $raw_data = price_check_decode_field($node->field_subscribers['und']);
  foreach($raw_data as $key => $subscription) {
    if(!$ebay_id) {
      $subscriber = price_check_get_subscriber_from_string($subscription);
      $ebay_account = $subscriber['ebay_account'];
      $ebay_id = $subscriber['ebay_id'];
      $ebay_accounts = price_check_get_ebay_account_names();
      if(in_array($ebay_account, $ebay_accounts)) {
        //id, account matches
        $raw_data[$key] = "";
        price_check_cancel_ebay_item($ebay_account, $ebay_id);
      }
      continue;
    }
    if(strpos($subscription, "|$ebay_id") !== false) {
      $subscriber = price_check_get_subscriber_from_string($subscription);
      $ebay_account = $subscriber['ebay_account'];
      $ebay_accounts = price_check_get_ebay_account_names();
      if(in_array($ebay_account, $ebay_accounts)) {
        //id, account matches
        $raw_data[$key] = "";
        price_check_cancel_ebay_item($ebay_account, $ebay_id);
      }
    }
  }
  $output = array();
  foreach($raw_data as $raw_data_item) {
    if($raw_data_item) $output[]['value'] = $raw_data_item;
  }
  if((count($output) == 0) && ($node->uid == $user->uid)) {
    node_delete($nid);
  } else {
    $node->field_subscribers['und'] = $output;
    node_save($node);
  }
  drupal_set_message('Listing removed from DS Genie');
  $ref = @$_SERVER["HTTP_REFERER"];
  if(!$ref) {
    $ref = '/';
    drupal_set_message('HTTP REFERER not set, redirecting to Homepage.', 'warning');
  }
  header("Location: " . $ref); exit(0);
} 

function price_check_lookup_amazon_form($form, &$form_state) {
  $form['url'] = array(
    '#type' => 'textfield', 
    '#title' => 'Amazon URL?',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  if(@$_SESSION['amazon_output']) {
    $form['results'] = array(
      '#type' => 'item', 
      '#title' => 'Results',
      '#markup' => @$_SESSION['amazon_output'][0],
    );
    unset($_SESSION['amazon_output']);
  }
  return $form;
}

function price_check_lookup_amazon_form_submit(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $url = $form_state['values']['url'];
  $asin = price_check_get_amazon_asin_from_url($url);
  if(!$asin) {
    $_SESSION['amazon_output'] = array("Unable to extract ASIN, check your URL.");
    return;
  }
  $item = price_check_get_amazon_item_from_asin($asin);  
  
  if(@$_GET['debug']) {
    print_r($item); exit(0);
  }
  
  if(!$item) {
    $_SESSION['amazon_output'] = array("URL failed amazon lookup, check your URL.");
    global $user;
    if($user->uid == 1) {
      $_SESSION['amazon_output'] = array(print_r($item, true));
    }
    return;
  }
  //if we have a parent, follow it
  if(@$item['Item']['ParentASIN'] && ($item['Item']['ParentASIN'] != $item['Item']['ASIN'])) {
    $item = price_check_get_amazon_item_from_asin($item['Item']['ParentASIN']);
  }
  global $user;
  
  //we are at root level now, extract data
  $base_url = urldecode($item['Item']['DetailPageURL']);
  $output = "<table><tr><th>Title</th><th>Image</th><th>UPC</th><th>MPN</th><th>Brand</th><th>Option</th><th>Price</th><th>URL</th><th>Availibility</th><th>Add item</th></tr>";
  if(@$item['Item']['Variations']) {
    $variations = price_check_force_json_array($item['Item']['Variations']['Item']);
    foreach($variations as $variation) {
      //print_r($item['Item']['Variations']); exit(0);
      $url = "<a href='" . str_replace($variation['ParentASIN'], $variation['ASIN'], $base_url) . "' target='_blank'>right click and copy link address/location</a>";
      $title = $variation['ItemAttributes']['Title'];
      $image = @$variation['MediumImage']['URL'];
      $upc = @$variation['ItemAttributes']['UPC'];
      $mpn = @$variation['ItemAttributes']['MPN'];
      $brand = @$variation['ItemAttributes']['Brand'];
      $variation_attributes =  @$variation['VariationAttributes']['VariationAttribute'];
      $options_array = array();
      if($variation_attributes) {
        $variation_attributes = price_check_force_json_array($variation_attributes);
        foreach($variation_attributes as $variation_attribute) {
          $options_array[] = "{$variation_attribute['Name']}:{$variation_attribute['Value']}";
        }
      }
      $options = implode(', ', $options_array);
      $offers = @$variation['Offers']['Offer'];
      if($offers) {
        $offers = price_check_force_json_array($offers);
        $price = price_check_get_amazon_best_offer($offers);
      }
      if($price) 
        $availability = "Available";
      else 
        $availability = "Not Available";
      if($image) {
        $image_html = "<img src='$image' />";
      } else {
        $image_html = "N/A";
      }
      $output .= "<tr><td>$title</td><td>$image_html</td><td>$upc</td><td>$mpn</td>" .
          "<td>$brand</td><td>$options</td><td>\$$price</td><td>$url</td><td>$availability</td>" .
          "<td>&nbsp</td></tr>";
    }
  } else {
    //wrap item like a variation ;)
    $variations = price_check_force_json_array($item['Item']);
    foreach($variations as $variation) {
      $url = "<a href='$base_url' target='_blank'>right click and copy link address/location</a>";
      $title = $variation['ItemAttributes']['Title'];
      $image = $variation['MediumImage']['URL'];
      $upc = @$variation['ItemAttributes']['UPC'];
      $mpn = @$variation['ItemAttributes']['MPN'];
      $brand = @$variation['ItemAttributes']['Brand'];
      $variation_attributes =  @$variation['VariationAttributes']['VariationAttribute'];
      $options_array = array();
      if($variation_attributes) {
        $variation_attributes = price_check_force_json_array($variation_attributes);
        foreach($variation_attributes as $variation_attribute) {
          $options_array[] = "{$variation_attribute['Name']}:{$variation_attribute['Value']}";
        }
      }
      $options = implode(', ', $options_array);
      $offers = @$variation['Offers']['Offer'];
      if($offers) {
        $offers = price_check_force_json_array($offers);
        $price = price_check_get_amazon_best_offer($offers);
      }
      if(@$price) 
        $availability = "Available";
      else {
        $availability = "Not Available";
        $price = 0;
      }
      $output .= "<tr><td>$title</td><td><img src='$image' /></td><td>$upc</td><td>$mpn</td>" .
          "<td>$brand</td><td>$options</td><td>\$$price</td><td>$url</td><td>$availability</td>" .
          "<td>&nbsp</td></tr>";
    }
  } 
  $output .= "</table>";
  $_SESSION['amazon_output'] = array($output);
}

function price_check_ebay_listings_user_report_b($success = 'not started', $results = array(), $operations = array())   {
  if(@$_POST['default_quantity']) {
    global $user;
    $uid = $user->uid;
    $work = array();
    foreach($_POST as $key => $val) {
      if($key != 'default_quantity') {
        $ebay_id = substr($key, 1);
        $work[] = $ebay_id;
      }
    }
    if($work) {
      $result = price_check_set_item_quantities($uid, $work, $_POST['default_quantity']);
      if($result['Ack'] == 'Failure') {
        return price_check_translate_xml_errors($result['Errors']);
      }
      /*
       * <ReviseInventoryStatusResponse xmlns="urn:ebay:apis:eBLBaseComponents">
  <!-- Call-specific Output Fields -->
  <Fees> InventoryFeesType
    <Fee> FeeType
      <Fee currencyID="CurrencyCodeType"> AmountType (double) </Fee>
      <Name> string </Name>
      <PromotionalDiscount currencyID="CurrencyCodeType"> AmountType (double) </PromotionalDiscount>
    </Fee>
    <!-- ... more Fee nodes allowed here ... -->
    <ItemID> ItemIDType (string) </ItemID>
  </Fees>
  <!-- ... more Fees nodes allowed here ... -->
  <InventoryStatus> InventoryStatusType
    <ItemID> ItemIDType (string) </ItemID>
    <Quantity> int </Quantity>
    <SKU> SKUType (string) </SKU>
    <StartPrice currencyID="CurrencyCodeType"> AmountType (double) </StartPrice>
  </InventoryStatus>
       */
      $items = price_check_force_json_array($result['InventoryStatus']);
      $output = array();
      foreach($items as $item) {
        $output[] = "{$item['ItemID']} : {$item['Quantity']}";
      }
      return implode('<br />', $output);
    }
  }
  $output = @$_SESSION['price_check_ebay_listings_user_report_b'][0];
  if($output) {
    unset($_SESSION['price_check_ebay_listings_user_report_b']);
    return $output;
  } else {
    global $user;
    $uid = $user->uid;
    $sql = <<<EOSQL
select fs.entity_id, field_subscribers_value from node n join 
field_data_field_subscribers fs on fs.entity_id = n.nid
join field_data_field_store_price fsp on nid = fsp.entity_id
join field_data_field_price fp on nid = fp.entity_id
where 
uid = :uid;
EOSQL;
    $result = db_query($sql, array(":uid" => $uid));
    $operations = array();
    foreach($result as $record) {
      $operations[] = array('price_check_ebay_listings_user_report_b_atom',
        array($record));
    }
    $batch = array(
      'title' => 'Loading your Ebay Items (only those tracked by DS Genie)',
      'operations' => $operations,
      'finished' => 'price_check_ebay_listings_user_report_b_finished',
      'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
    );
    batch_set($batch);
    batch_process('my_listings_report_b');
  }
}

function price_check_ebay_listings_user_report_b_atom($record, &$context) {
  $nid = $record->entity_id;
  $params = $record->field_subscribers_value;
  $subscriber = price_check_get_subscriber_from_string($params);
  $ebay_id = $subscriber['ebay_id'];
  $item = node_load($nid);
  if(!$item) return;
  $pictures = price_check_decode_field(@$item->field_picture['und']);
  if(count($pictures) > 0) {
    $picture = $pictures[0];
  } else {
    $pictures = price_check_decode_picture_field(@$item->field_ds_genie_pictures['und']);
    if(count($pictures) > 0) $picture = $pictures[0];
  }
  if(@$picture) {
    $img = "<img src='$picture' width='100' />";
  } else 
    $img = "N/A";
  //$sugg_price = @$item->field_price['und'][0]['value'];
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  $quantity = $ebay_item['Item']['Quantity'];
  $ebay_price = $ebay_item['Item']['StartPrice'];
  $item->field_price['und'][0]['value'] = $ebay_price;
  node_save($item);
  $src_price = $item->field_store_price['und'][0]['value'];
  $markup = number_format(($ebay_price - $src_price) * 100 / $src_price, 2);
  $profit = number_format(($ebay_price  - ($ebay_price * 0.029)  - 0.3 - ($ebay_price * 0.09) - $src_price) * 100 / $src_price, 2);
  if(!$ebay_item) return;
  if($quantity == 0) {
    $checkbox = "<input type='checkbox' name='n$ebay_id' checked />";
  } else 
    $checkbox = "";
  
  $output = "<tr><td>$img</td>" .
      "<td><a href='" . price_check_get_ebay_item_url($ebay_id) . "'>". @$item->title . "</a></td>" .
      "<td>$ebay_price</td>" .
      "<td>$src_price</td>" .
      "<td>$markup%</td>" .
      "<td>$profit%</td>" .
      "<td>$quantity</td>" .
      "<td>$checkbox</td>" .
      "<td><a href='/node/$nid/edit?rawform=1&check_ebay=1'>edit</a></td></tr>";
  $context['results'][] = $output;
}

function price_check_ebay_listings_user_report_b_finished($success, $results, $operations)   {
  $output = "<p>You have no listings at this time.</p>";
  if($results) {
    $output = "<form method='POST'>" .
      "<table><tr><th>Image</th><th>Item Name</th><th>Ebay Price</th><th>Src Price</th><th>Markup</th><th>Profit</th><th>Quantity</th><th>Replenish</th><th>edit</th></tr>";
    foreach($results as $result) {
      $output .= $result;
    }
    $output .= '<input type="text" id="default_quantity" name="default_quantity" value="" size="8" maxlength="8" class="form-text">' .
      '<input type="submit" id="default_quantity_submit" name="default_quantity_submit" value="Update" class="form-submit">' .
      "</form></table>";
  }
  $_SESSION['price_check_ebay_listings_user_report_b'] = array($output);
}

function price_check_resize_images() {
  global $user;
  $uid = $user->uid;
  $sql = <<<EOSQL
select field_picture_value from node n join
field_data_field_picture fp on fp.entity_id = n.nid
where field_picture_value like 'http://dsgenie.com/sites/default/files/file%'
and uid = :uid
EOSQL;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array();
  foreach($results as $row) {
    $operations[] = array('price_check_resize_image_atom',
      array($row->field_picture_value));
  }
  $batch = array(
    'title' => 'Resizing images',
    'operations' => $operations,
    'finished' => 'price_check_resize_image_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('/');
}

function xprice_check_resize_image_atom($image_url, &$context) {
  $parts = explode('/', $image_url);
  $filename = '/var/www/html/sites/default/files/' . array_pop($parts);
  //DEVELOPMENT PAUSED
}

function price_check_operation_resize($source_image)
{
    $info = getimagesize($source_image);
    $imgtype = image_type_to_mime_type($info[2]);

    #assuming the mime type is correct
    switch ($imgtype) {
        case 'image/jpeg':
            $source = imagecreatefromjpeg($source_image);
            break;
        case 'image/gif':
            $source = imagecreatefromgif($source_image);
            break;
        case 'image/png':
            $source = imagecreatefrompng($source_image);
            break;
        default:
            die('Invalid image type.');
    }

    #Figure out the dimensions of the image and the dimensions of the desired thumbnail
    $src_w = imagesx($source);
    $src_h = imagesy($source);
    
    if(($src_w > 500) || ($src_h > 500)) return true;
    
    $tn_w = ($src_w < 500) ? 500 : $src_w;
    $tn_h = ($src_h < 500) ? 500 : $src_h;

    #Do some math to figure out which way we'll need to crop the image
    #to get it proportional to the new size, then crop or adjust as needed

    $x_ratio = $tn_w / $src_w;
    $y_ratio = $tn_h / $src_h;

    if (($src_w <= $tn_w) && ($src_h <= $tn_h)) {
        $new_w = $src_w;
        $new_h = $src_h;
    } elseif (($x_ratio * $src_h) < $tn_h) {
        $new_h = ceil($x_ratio * $src_h);
        $new_w = $tn_w;
    } else {
        $new_w = ceil($y_ratio * $src_w);
        $new_h = $tn_h;
    }

    $newpic = imagecreatetruecolor(round($new_w), round($new_h));
    imagecopyresampled($newpic, $source, 0, 0, 0, 0, $new_w, $new_h, $src_w, $src_h);
    $final = imagecreatetruecolor($tn_w, $tn_h);
    $backgroundColor = imagecolorallocate($final, 255, 255, 255);
    imagefill($final, 0, 0, $backgroundColor);
    imagecopy($final, $newpic, (($tn_w - $new_w)/ 2), (($tn_h - $new_h) / 2), 0, 0, $new_w, $new_h);

    if (imagejpeg($final, $source_image, 100)) {
      return true;
    }
    return false;
}

function price_check_universal_lookup_form($form, &$form_state) {
  $form['vendor_url'] = array(
    '#type' => 'textfield', 
    '#title' => 'URL',
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE, 
  );
  $form['vendor_variant'] = array(
    '#type' => 'textfield', 
    '#title' => 'Vendor Variant',
    '#size' => 80,
    '#maxlength' => 255,
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  if(isset($_SESSION['universal_lookup_output']) && count($_SESSION['universal_lookup_output'])) {
    $form['results'] = array(
      '#type' => 'item', 
      '#title' => 'Results',
      '#markup' => $_SESSION['universal_lookup_output'][0],
    );
    $_SESSION['universal_lookup_output'] = array();
  }
  return $form;
}

function price_check_universal_lookup_form_submit(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $vendor_url = @$form_state['values']['vendor_url'];
  $vendor_variant = @$form_state['values']['vendor_variant'];
  
  $skugrid_obj_raw = price_check_skugrid_get_item($vendor_url, $vendor_variant);
  $skugrid_obj = $skugrid_obj_raw->Item[0];
  //print_r($skugrid_obj); exit();
  /*
   * stdClass Object
(
    [Item] => Array
        (
            [0] => stdClass Object
                (
                    [local_id] => 1840373
                    [remote_id] => 9802492
                    [compare_id] => 9802493
                    [reprice_id] => 0
                    [ext_user] => dsgeniemarc
                    [reference] => 
                    [price] => 0
                    [shipping] => 0
                    [fee] => 0
                    [profit_formula] => 
                    [selling_formula] => 
                    [last_stock] => 0
                    [last_price] => 0
                    [last_shipping] => 0
                    [last_active] => 2016-03-21 17:54:00
                    [deleted] => 0
                    [date_added] => 2016-03-21 20:54:04
                    [vendor] => WALMART
                    [vendor_url] => http://www.walmart.com/ip/Pediacare-Children-s-Grape-Flavor-Fever-Reducer-Pain-Reliever-Plus-Multi-Sympton-Cold-4-oz/17248577
                    [vendor_sku] => 17248577
                    [vendor_variant] => 
                    [vendor_stock] => 1
                    [vendor_price] => 5.78
                    [vendor_shipping] => 4.97
                    [vendor_title] => Pediacare Children's Grape Flavor Fever Reducer/Pain Reliever Plus Multi-Sympton Cold, 4 Oz
                    [vendor_image] => http://i5.walmartimages.com/dfw/dce07b8c-2afb/k2-_f9415da3-13dc-4a9d-9a5b-7d92547c53b3.v1.jpg
                    [retail_price] => 0
                    [date_updated] => 2016-03-21 20:54:11
                    [date_update_started] => 0000-00-00 00:00:00
                    [failsafe] => 0
                    [errors] => 0
                    [compare_vendor] => Manual Entry
                    [compare_sku] => 
                    [compare_url] => #14586080446762
                    [compare_variant] => 
                    [compare_price] => 0
                    [compare_shipping] => 0
                    [reprice_sku] => 
                    [reprice_store] => 
                    [reprice_pause] => 0
                )

        )

    [Ack] => Success
)
   */
  
  
  /*
      [local_id] => 1840373
      [vendor_variant] => 
      [vendor_stock] => 1
      [vendor_price] => 5.78
      [vendor_shipping] => 4.97
      [vendor_sku] => 17248577
      [vendor_title] => Pediacare Children's Grape Flavor Fever Reducer/Pain Reliever Plus Multi-Sympton Cold, 4 Oz
      [vendor_image] => http://i5.walmartimages.com/dfw/dce07b8c-2afb/k2-_f9415da3-13dc-4a9d-9a5b-7d92547c53b3.v1.jpg
      
   */
  
  $obj = new stdClass();
  $obj->localId = $skugrid_obj->local_id;
  $obj->title = $skugrid_obj->vendor_title;
  $obj->image = $skugrid_obj->vendor_image;
  $obj->vendorVariant = $skugrid_obj->vendor_variant;
  $obj->salePrice = $skugrid_obj->vendor_price;
  $obj->standardShipRate = $skugrid_obj->vendor_shipping;
  if($skugrid_obj->vendor_stock != 1)  
    $obj->stock = "Not Available";
  else 
    $obj->stock = "Available";
  $obj->itemId = $skugrid_obj->vendor_sku;
  
  //$obj->upc = "<a href='#' onclick=\"javascript:jQuery(this.parentElement).load('/walmart_upc/{$obj->itemId}');\">click</a>";
  $results = array();
  $results[] = "<!-- " . print_r($obj, true) . " -->";
  $results[] = "<table><tr><th>&nbsp</th><th>Name</th><th>Variant</th><th>Price</th><th>Shipping</th><th>Stock</th></tr>";
  $results[] = "<tr><td><img src='{$obj->image}' /></td>" .
    "<td>{$obj->title}</td>" .
    "<td>{$obj->vendorVariant}</td>" .
    "<td>{$obj->salePrice}</td>" .
    "<td>{$obj->standardShipRate}</td>" .
    "<td>{$obj->stock}</td>" .
    "</tr>";
  $results[] = "</table>";
  $_SESSION['universal_lookup_output'] = array(implode('', $results));
}

function price_check_convert_skugrid($nid) {
  $node = node_load($nid);
  $url = @$node->field_external_url['und'][0]['value'];
  $obj = price_check_get_item_object_from_url($url, true);
  $skugrid_variant = @$obj->skugrid_variant;
  $skugrid_obj = price_check_skugrid_get_item($url, $skugrid_variant);
  $skugrid_id = @$skugrid_obj->Item[0]->local_id;
  if($skugrid_id) {
    $node->field_skugrid_local_id['und'][0]['value'] = $skugrid_id;
    if($skugrid_variant) $node->field_supplier_variation_name['und'][0]['value'] = $skugrid_variant;
    node_save($node);
    //$results = array("success" => 1, "id" => $skugrid_id);
    return "#$skugrid_id";
  } else {
    //$results = array("success" => 0);
    return "failed";
  }
}

function price_check_convert_skugrid_ajax($content) {
  print $content;
}