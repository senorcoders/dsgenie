<?php

function price_check_spider_admin() {
  return price_check_spider_dashboard();
}

function price_check_spider_dashboard() {
  $output = "<table><th>Name</th><th>Action</th></tr>";
  $sql = "select title,nid from node where type='spider'";
  $results = db_query($sql);
  foreach($results as $row) {
    $title = $row->title;
    $nid = $row->nid;
    $view_link = price_check_make_link("/view_spider/$nid", $title);
    $edit_link = price_check_make_link("/node/$nid/edit", 'edit');
    $output .= price_check_make_table_row(array($view_link, $edit_link));
  }
  $output .= "</table>";
  return $output;
}

function price_check_get_active_spider() {
  $sql = "select entity_id from field_data_field_spider_active where field_spider_active_value = 1";
  $result = db_query($sql);
  $nid = $result->fetchField();
  return $nid;
}
function price_check_start_spider() {
  $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
  $queue->createQueue();
  //$q = array('nid' => 60385, 'op' => 'fix_spider');
  //$queue->createItem($q); 
  $queue->createItem(60385);  
}

function price_check_spider_debug() {
  $output = '<table><tr><th>id</th><th>data</th></tr>';
  $sql = "select * from queue where name = 'price_check_spider_queue_atom'";
  $results = db_query($sql);
  foreach($results as $row) {
    $id = $row->item_id;
    $data = $row->data;
    $output .= "<tr><td>$id</td><td>$data</td></tr>";
  }
  $output .= "</table>";
  
  $sql = "select * from watchdog where type like 'queue%'";
  $output .= '<table><tr><th>id</th><th>type</th><th>message</th></tr>';
  $results = db_query($sql);
  foreach($results as $row) {
    $id = $row->wid;
    $type = $row->type;
    $message = $row->message;
    $output .= "<tr><td>$id</td><td>$type</td><td>$message</td></tr>";
  }
  $output .= "</table>";
  return $output;
}

function price_check_spider_queue_atom($item) {
  if(!is_array($item)) {
    $node = node_load($item);
    $seller_raw = @array_pop($node->field_sellers['und']);
    if($seller_raw) {
      $seller = $seller_raw['value'];
      $sellers_processed = price_check_decode_field($node->sellers_processed['und']);
      if(!in_array($seller, $sellers_processed)) {
        $node->field_sellers_processed['und'][] = array('value' => $seller);
        $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
        $queue->createQueue();  
        $q = array('nid' => $node->nid, 'op' => 'seller_items', 'data' => $seller, 'arg1' => 1);
        $queue->createItem($q); 
        watchdog("queue:seller", print_r($q, true));
      }
      node_save($node);
    } else {
      return;
    }
  } else {
    $node = node_load($item['nid']);
    $op = $item['op'];
    $data = @$item['data'];
    $arg1 = @$item['arg1'];
    $arg2 = @$item['arg2'];
    if($op =='fix_spider') {
      //sellers processed
      $sellers_processed = price_check_decode_field($node->field_sellers_processed['und']);
      $clean_sellers_processed = array();
      foreach($sellers_processed as $seller) {
        if(!in_array($seller, $clean_sellers_processed)) $clean_sellers_processed[] = $seller;
      }
      $node->field_sellers_processed['und'] = price_check_encode_field($clean_sellers_processed);  
      
      //titles processed
      $titles_processed = price_check_decode_field($node->field_titles_processed['und']);
      $clean_titles_processed = array();
      foreach($titles_processed as $title) {
        if(!in_array($title, $clean_titles_processed)) $clean_titles_processed[] = $title;
      }
      $node->field_titles_processed['und'] = price_check_encode_field($clean_titles_processed); 
      
      //urls processed
      $urls_processed = price_check_decode_field($node->field_urls_processed['und']);
      $clean_urls_processed = array();
      foreach($urls_processed as $title) {
        if(!in_array($title, $clean_urls_processed)) $clean_urls_processed[] = $title;
      }
      $node->field_urls_processed['und'] = price_check_encode_field($clean_urls_processed); 
      
      node_save($node);
      return;
    }
    if($op == 'seller_items') {
      $seller = $data;
      $page = $arg1;
      $data = price_check_get_ebay_seller_items($seller, $page);
      $items = price_check_force_json_array($data['searchResult']['item']);
      $pages = $data['paginationOutput']['totalPages'];
      $results = $data['paginationOutput']['totalEntries'];
      if($results == 0) {
        $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
        $queue->createQueue();  
        $queue->createItem($node->nid);
        watchdog("queue:seller_items0", print_r($data, true));
        return;
      }
      if($pages > 1 && $page == 1) {
        for($i=2;$i<=$pages;$i++) {
          $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
          $queue->createQueue();  
          $q = array('nid' => $node->nid, 'op' => 'seller_items', 'data' => $seller, 'arg1' => $i);
          $queue->createItem($q);  
          //watchdog("queue:seller_items", print_r($q, true));
        }
      }
      foreach($items as $item) {
        if(price_check_get_item_sales_30($item) >= $node->field_threshold['und'][0]['value']) {
          //save if above threshold
          $urls_processed = price_check_decode_field(@$node->field_urls_processed['und']);
          $url = $item['viewItemURL'];
          if(!in_array($url, $urls_processed)) {
            $node->field_urls_processed['und'][] = array('value' => $url);
          }
        }
        $title = $item['title'];
        $titles = price_check_decode_field(@$node->field_titles_processed['und']);
        if(!in_array($title, $titles)) {
          $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
          $queue->createQueue();  
          $q = array('nid' => $node->nid, 'op' => 'title_search', 'data' => $title);
          //$queue->createItem($q); 
          //watchdog("queue:seller_add_title", print_r($q, true));
          //$node->field_titles_processed['und'][] = array('value' => $title);
        }
      }
      node_save($node);
    }
    if($op == 'title_search') {
      $title = $data;
      $titles = price_check_decode_field($node->field_titles_processed['und']);
      if(!in_array($title, $titles)) {
        $results = price_check_get_items_by_keywords($title);
        $items = $results['searchResult']['item'];
        if(@$items) {
          if(@$items['itemId']) $items = array($items);
          $output = array();
          $queue = DrupalQueue::get('price_check_spider_queue_atom'); 
          $queue->createQueue();  
          foreach($items as $item) {
            $seller = $item['sellerInfo']['sellerUserName'];
            $sellers_processed = price_check_decode_field($node->field_sellers_processed['und']);
            if(!in_array($seller, $sellers_processed)) {
              $sellers_processed = price_check_decode_field($node->field_sellers['und']);
              if(!in_array($seller, $sellers_processed)) {
                $node->field_sellers['und'][] = array('value' => $seller);
                $queue->createItem($node->nid); 
                watchdog("queue:title_court", print_r($node->nid, true));
              }
            }
            
            $title = $item['title'];
            $titles = price_check_decode_field($node->field_titles_processed['und']);
            if(!in_array($title, $titles)) {
              $q = array('nid' => $node->nid, 'op' => 'title_search', 'data' => $title);
              $queue->createItem($q); 
              watchdog("queue:seller_add_title", print_r($q, true));
              $node->field_titles_processed['und'][] = array('value' => $title);
            }
          }
          node_save($node);
        }
      }
    }
  }
}