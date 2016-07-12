<?php

function price_check_ebay_info() {
  $data = price_check_get_ebay_info();
  print_r($data); exit(0);
}

function price_check_edit_ebay_item($ebay_id) {
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  $seller_ebay_account = @$ebay_item['Item']['Seller']['UserID'];
  $ebay_accounts = price_check_get_ebay_account_names();
  if(in_array($seller_ebay_account, $ebay_accounts)) {
    $node = price_check_get_item_template_from_ebay_id($ebay_id);
    if($node) {
      //update node
      global $user;
      $node->title = $ebay_item['Item']['Title'];
      $node->body['und'][0]['value'] = $ebay_item['Item']['Description'];
      $node->body['und'][0]['format'] = 'ebay';
      $node->field_price['und'][0]['value'] = $ebay_item['Item']['StartPrice'];
      $node->field_dispatch_time['und'][0]['value'] = $ebay_item['Item']['DispatchTimeMax'];
      /*$subscribers = price_check_decode_field($node->field_subscribers['und']);
      $subscriber = array('ebay_account' => $seller_ebay_account,
        'ebay_id' => $ebay_id,);
      foreach($subscribers as $key => $val) {
        if(strpos($val, "{$subscriber['ebay_account']}|{$subscriber['ebay_id']}") !== false) {
          $node->field_subscribers['und'][$key]['value'] = "{$subscriber['ebay_account']}|{$subscriber['ebay_id']}";
        }    
      }*/
      $pictures = price_check_force_json_array($ebay_item['Item']['PictureDetails']['PictureURL']);
      $picture_output = array();
      //print_r($pictures); exit(0);
      if($pictures) {
        foreach($pictures as $picture) {
          $picture_output[] = array('value' => price_check_save_ebay_picture($picture));
        }
        $node->field_picture['und'] = $picture_output;
      }
      $item_specifics_raw = @$ebay_item['Item']['ItemSpecifics']['NameValueList'];
      $item_specifics = array();
      if($item_specifics_raw)
        foreach($item_specifics_raw as $item_specific) {
          $item_specifics[$item_specific['Name']] = $item_specific['Value'];
        }
      $node->field_ebay_upc[$node->language][0]['value'] = @$item_specifics['UPC'];
      $node->field_ebay_mpn[$node->language][0]['value'] = @$item_specifics['MPN'];
      $node->field_ebay_brand[$node->language][0]['value'] = @$item_specifics['Brand'];
      $node->field_ebay_item_id[$node->language][0]['value'] = $ebay_id;
      $node = node_submit($node); 
      node_save($node);
      header("Location: /node/{$node->nid}/edit?rawform=1");
    } else {
      return "This item is not being tracked in dsgenie.";
    }
  } else { 
    return "This is not your item.";
  }
}

function price_check_listing_report() {
  $sql = <<<EOSQL
select count(*) num, field_user_full_name_value, date_format(from_unixtime(field_complete_date_value), '%j') doy, date_format(from_unixtime(field_complete_date_value), '%d/%m/%Y') date, n.uid, field_assigned_to_value, field_complete_value from field_data_field_assigned_to ft
join field_data_field_complete fc on fc.entity_id = ft.entity_id
join node n on n.nid = fc.entity_id
join field_data_field_complete_date fd on fd.entity_id = fc.entity_id
join field_data_field_user_full_name fn on fn.entity_id = n.uid
where field_assigned_to_value > ''
group by uid,date_format(from_unixtime(field_complete_date_value), '%j')
order by date_format(from_unixtime(field_complete_date_value), '%j') desc , uid
EOSQL;
  $results = db_query($sql);
  $date = '';
  $output = '';
  $table_header = "<table><tr><th>Name</th><th>Count</th></tr>";
  $table_footer = "</table>";
  $first = true;
  foreach($results as $row) {
    $current_date = $row->date;
    if($date != $current_date) {
      if($first) {
      $first = false;
      } else {
        $output .= $table_footer;
      }
      $date = $current_date;
      $output .= "<h2>$date</h2>";
      $output .= $table_header;
    }
    $output .= "<tr><td>{$row->field_user_full_name_value}</td><td>{$row->num}</td></tr>";
  }
  $output .= $table_footer;
  return $output;
}

function price_check_work_report() {
  $sql = <<<EOSQL
select count(*) num, field_user_full_name_value, date_format(from_unixtime(field_complete_date_value), '%j') 
  doy, date_format(from_unixtime(field_complete_date_value), '%d/%m/%Y') date, 
    n.uid, field_assigned_to_value, field_complete_value from field_data_field_assigned_to ft
join field_data_field_complete fc on fc.entity_id = ft.entity_id
join node n on n.nid = fc.entity_id
join field_data_field_complete_date fd on fd.entity_id = fc.entity_id
join field_data_field_user_full_name fn on fn.entity_id = field_assigned_to_value
where field_assigned_to_value > ''
and cast(field_complete_date_value as unsigned) >= 1451606400
group by field_assigned_to_value,date_format(from_unixtime(field_complete_date_value), '%j')
order by date_format(from_unixtime(field_complete_date_value), '%j') desc , field_assigned_to_value
EOSQL;
  $results = db_query($sql);
  $date = '';
  $output = '';
  $table_header = "<table><tr><th>Name</th><th>Count</th></tr>";
  $table_footer = "</table>";
  $first = true;
  foreach($results as $row) {
    $current_date = $row->date;
    if($date != $current_date) {
      if($first) {
      $first = false;
      } else {
        $output .= $table_footer;
      }
      $date = $current_date;
      $output .= "<h2>$date</h2>";
      $output .= $table_header;
    }
    $output .= "<tr><td>{$row->field_user_full_name_value}</td><td>{$row->num}</td></tr>";
  }
  $output .= $table_footer;
  return $output;
}

function price_check_review_listings() {
  $sql = <<<SQL
select nid, title, field_user_full_name_value from field_data_field_complete fc 
join field_data_field_imported fi on fi.entity_id = fc.entity_id  
join node on node.nid = fc.entity_id
join field_data_field_assigned_to ft on ft.entity_id = node.nid
join field_data_field_user_full_name fn on fn.entity_id = field_assigned_to_value
where field_complete_value = 1 and 
uid in (5,1,2290)
SQL;
  $results = db_query($sql);
  $output = "<table><tr><th>Title</th><th>Worker</th><th>Action</th></tr>";
  foreach($results as $row) {
    $link = "<a href='/node/{$row->nid}/edit?rawform=1'>edit</a>";
    $output .= "<tr><td>{$row->title}</td><td>{$row->field_user_full_name_value}</td><td>$link</td></tr>";
  }
  $output .= "</table>";
  return $output;
}

function price_check_my_bot() {
  global $user;
  $uid = $user->uid;
  $name = $user->name;
  $sql = "select nid from node where type ='bot' and uid = :uid";
  $result = db_query($sql, array("uid" => $uid));
  $nid = @$result->fetchField();
  if(!$nid) {
    //create bot
    $node = new stdClass();
    $node->title = $title;
    $node->type = "bot";
    node_object_prepare($node); 
    $node->language = LANGUAGE_NONE; 
    $node->uid = $uid;
    $node->status = 1; 
    $node->promote = 0; 
    $node->comment = 0; 
    $node->title = "$name's bot";
    /*$node->field_sellers_raw
    $node->field_sellers
    $node->field_min_sales
    $node->field_min_price
    $node->field_api_limit
    $node->field_messages
    $node->field_api_usage
    $node->field_seller_index
    $node->field_start_seller_index*/
    $node = node_submit($node); // Prepare node for saving
    node_save($node);
    $nid = $node->nid;
  }
  $node = node_load($nid);
  $output = "<h3>My Bot</h3>";
  $output .= '';
}

function price_check_query_role_join_alter(QueryAlterableInterface $query) {
  $role = user_role_load_by_name("Paying Customer");
  $query->join('users_roles', 'r', 'users.uid = r.uid and r.rid = ' . $role->rid);  
}

function price_check_customers_devel() {
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
    'Subscription Ends',
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
      $rows[] = array(
        $account->field_user_full_name['und'][0]['value'],
        $account->name,
        implode(',', price_check_decode_field($account->field_ebay_accounts['und'])),
        $end_date,
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

function price_check_auto_work() {
  $sql = <<<EOSQL
select nid  from node where type = 'item_template' and uid = 5
and nid not in (select entity_id from field_data_field_assigned_to)
and nid not in (select entity_id from field_data_field_subscribers)
and nid not in (select entity_id from field_data_field_external_url)
and nid not in (select entity_id from field_data_field_suggested_accounts)
and nid > 69575
EOSQL;
  $results = db_query($sql);
  foreach($results as $row) {
    $rows[] = $row->nid;
  }
  $active_workers = price_check_get_active_workers();
  if($worker_count = count($active_workers)) {
    $workload = count($results) / $worker_count;
    $partitions = price_check_partition($rows, $worker_count);
    foreach($active_workers as $i => $worker) {
      price_check_assign_work($worker, @$partitions[$i]);
    }
  }
  exit(0);
  
  
  
  foreach($results as $row) {
    $nid = $row->nid;
    //$path = drupal_lookup_path('alias', "node/$nid");
    //if(!$path) continue;
    $item = node_load($nid);
    if($item) {
      $title = $item->title;
      $sql = <<<EOSQL
select count(*) from node where title = :title and nid > :nid and uid = 5
EOSQL;
      $result = db_query($sql, array('title' => $title, 'nid' => $nid));
      $count = $result->fetchField();
      if($count) {
        //$output[] = array($nid, $title);
        node_delete($nid);
        continue;
      }
    }
  }
  if(@$output) {
    print_r($output); exit(0);
  }
}








































function price_check_get_seller_items_multiple_form_a($form, &$form_state) {
  $user_full = price_check_get_current_user();
  if(!@$user_full->field_ebay_auth_token['und'][0]['value']) {
    drupal_set_message("Contact support to get your ebay API set up");
    return;  
  }
  $batch = @$_SESSION['price_check_get_seller_items_multiple_batch_a'][0];
  if($batch) {
    batch_set($batch);
    unset($_SESSION['price_check_get_seller_items_multiple_batch_a']);
    batch_process('get_seller_items_multiple');
  } else {
    $form['seller'] = array(
      '#type' => 'textarea', 
      '#title' => 'Seller',
      '#size' => 80,
      '#rows' => 20,
      '#required' => TRUE, 
    );
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
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
    if(@$_SESSION['get_seller_items_multiple_output_a']) {
      $count = count($_SESSION['get_seller_items_multiple_output_a']);
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
      unset($_SESSION['get_seller_items_multiple_output_a']);
    }
    return $form;  
  }
}

function price_check_get_seller_items_multiple_form_submit_a($form, &$form_state) {
  $sellers_raw = $form_state['values']['seller'];
  $min_sales = $form_state['values']['min_sales'];
  $min_price = @$form_state['values']['min_price'] + 0;
  if(!$min_price) $min_price = 0;
  $page = 1;
  $operations = array();
  $sellers = explode("\n", $sellers_raw);
  $sellers_blacklist = price_check_get_blacklist(true);
  foreach($sellers as $seller) {
    if(in_array(strtolower($seller), $sellers_blacklist)) continue;
    $operations[] = array(
      'price_check_batch_add_seller_items_multiple_request_a',
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
    'finished' => 'price_check_seller_items_multiple_finished_a',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('get_seller_items_multiple_a');
}

function price_check_batch_add_seller_items_multiple_request_a($seller, $page, $min_sales, $min_price, &$context) {
  $data = price_check_get_ebay_seller_items($seller, $page);
  $items = price_check_force_json_array($data['searchResult']['item']);
  $results = $data['paginationOutput']['totalEntries'];
  if(!$results) return;
  foreach($items as $item) {
    if($item['isMultiVariationListing'] == 'true') continue;
    if($item['autoPay'] == 'false') continue;
    if($item['sellingStatus']['currentPrice'] < $min_price) continue;
    $context['results'][] = array('item' => $item['itemId'], 'url' => $item['viewItemURL'], 'min_sales' => $min_sales);
  }
}

function price_check_seller_items_multiple_finished_a($success, $results, $operations)  {
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
    'title' => 'Step 2: Save Results',
    'operations' => $operations,
    'finished' => 'price_check_seller_items_search_multiple_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  $_SESSION['price_check_get_seller_items_multiple_batch'] = array($batch);
  return;
}

function price_check_seller_items_search_multiple_finished_a($success, $results, $operations)  {
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

function price_check_fix_ebay_images() {
  //for this user, load all listings, swap out images
  //only suggestions and suggested
  if(@$_SESSION['fix_images']) {
    unset($_SESSION['fix_images']);
    return "Finished fixing images.";
  }
  global $user;
  $uid = $user->uid;
  $sql = <<<EOSQL
select nid from 
node n where
type = 'item_template'
and uid = :uid
EOSQL;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array();
  foreach($results as $row) {
    $operations[] = array('price_check_fix_ebay_images_atom',
      array($row->nid),
    );
  }
  $batch = array(
    'title' => 'Fixing eBay images',
    'operations' => $operations,
    'finished' => 'price_check_fix_ebay_images_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
  );
  batch_set($batch);
  batch_process('fix_images');
}

function price_check_fix_ebay_images_atom($nid, &$context) {
  
  $node = node_load($nid);
  if($node) {
    if(@file_get_contents($node->field_picture['und'][0]['value'])) {
      $context['message'] = "Skip " . $node->title;
      return;
    }
    $ebay_url = $node->field_ebay_url['und'][0]['value'];
    $parts = explode("/", $ebay_url);
    $last_part = array_pop($parts);
    $ebay_id = array_shift(explode("?", $last_part));
    $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
    if(@!$ebay_item['Item']) {
      $raw_subscriber = @$node->field_subscribers['und'][0]['value'];
      if(!$raw_subscriber) {
        $context['message'] = "Cancelled/Expired listing " . $node->title;
        return;
      }
      $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
      $ebay_id = $subscriber['ebay_id'];
      $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
      if(!$ebay_item) {
        $context['message'] = "Cancelled/Expired listing+ " . $node->title;
        return;
      }
    }
    $pictures = $ebay_item['Item']['PictureDetails']['PictureURL'];
    $picture_output = array();
    if(is_array($pictures)) {
      foreach($pictures as $picture) {
        $picture_output[] = array('value' => price_check_save_ebay_picture($picture));
      }
    } else {
      $picture_output[] = array('value' => price_check_save_ebay_picture($pictures));
    }
    $node->field_picture['und'] = $picture_output;
    node_save($node);
    $context['message'] = $node->title;
  }
}

function price_check_fix_ebay_images_finished() {
  $_SESSION['fix_images'] = array(true);
}

function price_check_clear_research() {
  global $user;
  $user_full = price_check_get_current_user();
  if (in_array('Paying Customer', $user_full->roles)) {
  $sql = <<<SQL
select nid from node fa 
join field_data_field_seller fs on fs.entity_id = fa.nid
where fa.uid = :uid
and fs.entity_id not in (select entity_id from field_data_field_complete)
and fs.entity_id in (select entity_id from field_data_field_imported where field_imported_value = 1)
and fs.entity_id not in (select entity_id from field_data_field_complete where field_complete_value = 1)
SQL;
    $results = db_query($sql, array(':uid' => $user->uid));
    foreach($results as $row) {
      $nid = $row->nid;
      $operations[] = array(
        'price_check_clear_research_atom',
        array($nid),
      );
    }
    $batch = array(
      'title' => 'Remove Research',
      'operations' => $operations,
      'finished' => 'price_check_clear_research_finished',
      'file' => drupal_get_path('module', 'price_check')  . '/pages.inc',
    );
    batch_set($batch);
    batch_process('/');
  } else { 
    return;
  }
}

function price_check_clear_research_atom($nid, &$context) {
  node_delete($nid);
  $context['results'][] = $nid;
}
  
function price_check_clear_research_finished($success, $results, $operations) {
  $count = count($results);
  drupal_set_message("$count items removed.");
}

function price_check_unlink_items() {
  global $user;
  $uid = $user->uid;
  $sql =<<<EOSQL
select nid, field_subscribers_value from node n
join field_data_field_subscribers fs on fs.entity_id = n.nid
where type ='item_template'
and uid = :uid
EOSQL;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array();
  foreach($results as $row) {
    $operations[] = array(
      'price_check_unlink_items_atom',
      array($row->nid, $row->field_subscribers_value, $uid),
    );
  }
  $batch = array(
    'title' => 'Unlink Items',
    'operations' => $operations,
    'finished' => 'price_check_unlink_items_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.dev.inc',
  );
  batch_set($batch);
  batch_process('/');
}  

function price_check_unlink_items_atom($nid, $raw_subscriber, $uid, &$context) {
  $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
  $ebay_id = $subscriber['ebay_id']; 
  $item = price_check_get_ebay_item_from_id($ebay_id, $uid);
  if(!$item || $item['Item']['SellingStatus']['ListingStatus'] == 'Completed') {
    node_delete($nid);
    $context['message'] = "deleted $nid";
    $context['results'][] = $nid;
  }
}

function price_check_unlink_items_finished($success, $results, $operations) {
  $count = count($results);
  drupal_set_message("$count items removed");
}
  
function price_check_fix_items_custom() {
  global $user;
  $uid = $user->uid;
  $sql =<<<EOSQL
select nid, field_suggested_accounts_value from node n
join field_data_field_suggested_accounts fs on fs.entity_id = n.nid
where type ='item_template'
and uid = :uid
EOSQL;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array();
  foreach($results as $row) {
    $operations[] = array(
      'price_check_fix_items_custom_atom',
      array($row->nid, $row->field_subscribers_value, $uid),
    );
  }
  $batch = array(
    'title' => 'Relink Items',
    'operations' => $operations,
    'finished' => 'price_check_fix_items_custom_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.dev.inc',
  );
  batch_set($batch);
  batch_process('/');
}  

function price_check_fix_items_custom_atom($nid, $raw_subscriber, $uid, &$context) {
  $node = node_load($nid);
  //$node->field_subscribers = array();
  $node->field_suggested_accounts['und'][0]['value'] = 'moneysaver1985';
  $node->uid = 633;
  node_save($node);
  return;
  
  
  $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
  $ebay_id = $subscriber['ebay_id']; 
  $node = node_load($nid);
  $node->field_subscribers['und'][0]['value'] = "moneysaver1985|$ebay_id";
  node_save($node);
  /*$item = price_check_get_ebay_item_from_id($ebay_id, $uid);
  if(!$item || $item['Item']['SellingStatus']['ListingStatus'] == 'Completed') {
    node_delete($nid);
    $context['message'] = "deleted $nid";
    $context['results'][] = $nid;
  }*/
}

function price_check_fix_items_custom_finished($success, $results, $operations) {
  //$count = count($results);
  //drupal_set_message("$count items removed");
}
  
function price_check_cancelled_2_suggestion() {
  global $user;
  $uid = $user->uid;
  $sql = <<<EOSQL
select field_subscribers_value, entity_id from field_data_field_subscribers fs 
join node n on fs.entity_id = nid
where uid = :uid
EOSQL;
  $results = db_query($sql, array('uid' => $uid));
  $operations = array();
  foreach($results as $row) {
    $operations[] = array('price_check_cancelled_2_suggestion_atom',
      array($row->entity_id, 
        $row->field_subscribers_value,
      ));
  }
  $batch = array(
    'title' => 'Convert Cancelled Items to Suggestions',
    'operations' => $operations,
    'finished' => 'price_check_cancelled_2_suggestion_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/pages.dev.inc',
  );
  batch_set($batch);
  batch_process('/');
}

function price_check_cancelled_2_suggestion_atom($nid, $raw_subscriber, &$context) {
  $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
  $ebay_id = $subscriber['ebay_id'];
  $ebay_account = $subscriber['ebay_account'];
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  if(@$ebay_item['Item']['SellingStatus']['ListingStatus'] != 'Active') {
    $node = node_load($nid);
    $node->field_subscribers['und'] = array();
    $node->field_suggested_accounts['und'][] = array('value' => $ebay_account);
    node_save($node);
    $context['message'] = "{$node->title} converted.";
    drupal_set_message("{$node->title} converted.");
    $context['results'][] = $node->title;
  }
}

function price_check_cancelled_2_suggestion_finished($success, $results, $operations) {
  drupal_set_message(count($results) . " listings converted.");
}