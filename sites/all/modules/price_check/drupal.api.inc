<?php

function price_check_node_validate(&$node, $form, &$form_state) {
  if(@$node->nid == 591808) {
    print_r($node); exit(0);
  }
  
  
  global $user;
  if(@$_GET['override']) {
    return;
  }
  if($node->type == 'bot') {
    $sellers_raw = @$node->field_sellers_raw['und'][0]['value'];
    if($sellers_raw) { 
      $sellers_raw_arr = explode("\n", $sellers_raw);
      $sellers = price_check_decode_field($node->field_sellers['und'], true);
      foreach($sellers_raw_arr as $seller) {
        if(!in_array($seller, $sellers)) $sellers[] = $seller;
      }
      price_check_set_form_value($form, 'field_sellers_raw', '', $form_state);  
      price_check_set_form_value($form, 'field_sellers', price_check_encode_field($sellers), $form_state);  
    }
    return;
  }
  if($node->type == 'item_template') {
    if(@$form_state['input']['op'] == 'Save & Update Ebay') {
      price_check_revise_ebay_item($node);
      return;
    }
    if(@$form_state['input']['op'] == 'Delete') return;
    $stage = @$node->field_form_stage['und'][0]['value'];
    if(!@$node->nid) {
      $ebay_url = @$node->field_ebay_url['und'][0]['value'];
      if($ebay_url) {
        $parts = explode("/", $ebay_url);
        $last_part = array_pop($parts);
        $ebay_id = @array_shift(explode("?", $last_part));
        $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
        if(@$ebay_item['Item']) {
          $category_id = $ebay_item['Item']['PrimaryCategory']['CategoryID'];
          $category = price_check_get_category_info($category_id);
          //print_r($category); exit(0);
          $category_array = price_check_force_json_array($category['CategoryArray']['Category']);
            foreach($category_array as $category_item) {
            $category_path = $category_item['CategoryNamePath'];
            if(strpos($category_path, "eBay Motors") !== false) {
              form_set_error('field_ebay_url', "DsGenie does not support eBay Motors items");
              return;
            }
          }
          $ebay_price = $ebay_item['Item']['StartPrice'];
          price_check_set_form_value($form, 'field_price', $ebay_price, $form_state);    
          $ext_url = @$node->field_external_url['und'][0]['value'];
          $vendor_variant = @$node->field_supplier_variation_name['und'][0]['value'];
          if($ext_url) {
            //$ext_item = price_check_get_item_object_from_url($ext_url);
            $ext_item = price_check_skugrid_get_item_object($ext_url, $vendor_variant);
            if($ext_item && !is_array($ext_item)) {
              price_check_set_form_value($form, 'field_store_price', $ext_item->salePrice, $form_state);    
              price_check_set_form_value($form, 'field_shipping_price', $ext_item->standardShipRate, $form_state);    
              price_check_set_form_value($form, 'field_skugrid_local_id', $ext_item->localId, $form_state);    
              if($ext_item->stock != 'Available') {
                if(@$node->field_my_item['und'][0]['value']) {
                  drupal_set_message("Out of stock item", 'warning');
                } else {
                  form_set_error('field_external_url', "Out of stock item");
                }
              }
              price_check_set_form_value($form, 'field_store_status', $ext_item->stock, $form_state);  
              //on to validate the price
              $item_obj = $ext_item;
              if(!@$item_obj->salePrice) {
                form_set_error('field_external_url', "Invalid URL!");
                return;
              }
              //if(@$item_obj->standardShipRate) $item_obj->salePrice += $item_obj->standardShipRate;
              price_check_set_form_value($form, 'field_form_stage', 2, $form_state); 
              if(@$item_obj->upc) price_check_set_form_value($form, 'field_upc', $item_obj->upc, $form_state);
              if(@$item_obj->modelNumber) price_check_set_form_value($form, 'field_mpn', $item_obj->modelNumber, $form_state);
              if(@$item_obj->brandName) price_check_set_form_value($form, 'field_brand', $item_obj->brandName, $form_state);
              if(price_check_is_paying_customer() && @$node->field_my_item['und'][0]['value'] == 1) {
                //
              } else {
                if($item_obj->salePrice + $item_obj->standardShipRate - $ebay_price > 0) {
                  if(price_check_is_paying_customer()) {
                    drupal_set_message("Warning. Source price is too high.", 'warning');
                  } else {
                    form_set_error('field_external_url', "Source price is too high");
                    return;
                  }
                }
                $store_price = $item_obj->salePrice + $item_obj->standardShipRate;
                $fees = $ebay_price * (2.9 + 9)/100 + 0.30;
                $profit = $ebay_price - $store_price - $fees;
                $profit_percent = $profit*100/$store_price;
                if($profit_percent <= 5) {
                  if(price_check_is_paying_customer()) {
                    drupal_set_message("Warning. Profit is below 5% for this item.");
                  } else {
                    form_set_error('field_external_url', "Profit too low. Discard");
                    return;
                  }
                }
                if($profit_percent >= 10) {
                  $ebay_price = ($store_price * 11 + 3) / 8.81;
                  $ebay_price = number_format((float)$ebay_price, 2, '.', '');
                } else {
                  $ebay_price -= 1;
                }
              }
              price_check_set_form_value($form, 'field_price', $ebay_price, $form_state); 
            } else {
              if(is_array($ext_item)) {
                watchdog("problem with object", print_r(array('line/file' => __LINE__ ."/" . __FILE__ , 'ext_item' => $ext_item), true));
              }
              form_set_error('field_external_url', "Invalid/Unsupported URL");
            }
          } else 
            form_set_error('field_external_url', "Invalid/Unsupported URL");
          $form['title']['#parents'] = array('title'); 
          form_set_value($form['title'],$ebay_item['Item']['Title'], $form_state);
          if(!@$ebay_item['Item']['ConditionID']) $ebay_item['Item']['ConditionID'] = 1000;
          price_check_set_form_value($form, 'field_condition', $ebay_item['Item']['ConditionID'], $form_state);
          price_check_set_form_value($form, 'field_category', $ebay_item['Item']['PrimaryCategory']['CategoryID'], $form_state);
          $pictures = $ebay_item['Item']['PictureDetails']['PictureURL'];
          $picture_output = array();
          if(is_array($pictures)) {
            foreach($pictures as $picture) {
              $picture_output[] = array('value' => price_check_save_ebay_picture($picture));
            }
          } else {
            $picture_output[] = array('value' => price_check_save_ebay_picture($pictures));
          }
          price_check_set_form_value($form, 'field_picture', $picture_output, $form_state);
        } else {
          form_set_error('field_ebay_url', "Invalid Ebay URL");
          return;
        }
        //we got all this way... 
        if(price_check_is_paying_customer() && @$node->field_my_item['und'][0]['value'] == 1) {
          if(@$_GET['debug']) {
            print_r($ebay_item['Item']['Seller']); exit(0);
          }
          $seller_ebay_account = @$ebay_item['Item']['Seller']['UserID'];
          
          $ebay_accounts = price_check_get_ebay_account_names();
          if(in_array($seller_ebay_account, $ebay_accounts)) {
            $subscriber = "$seller_ebay_account|$ebay_id";
            price_check_set_form_value($form, 'field_subscribers', $subscriber, $form_state); 
            price_check_set_form_value($form, 'field_form_stage', 4, $form_state); 
          } else {
            form_set_error('field_ebay_url', "This is not your item.");
            return;
          }
        } 
      }
      return;
    }
    if(!$stage) {
      $ext_url = $node->field_external_url['und'][0]['value'];
      $vendor_variant = @$node->field_supplier_variation_name['und'][0]['value'];
      //$item_obj = price_check_get_item_object_from_url($ext_url);
      $item_obj = price_check_skugrid_get_item_object($ext_url, $vendor_variant);
      if($item_obj && !is_array($item_obj)) {
        //check the price
        if(!@$item_obj->salePrice) {
          form_set_error('field_external_url', "Invalid URL!");
          return;
        }
        //if(@$item_obj->standardShipRate) $item_obj->salePrice += $item_obj->standardShipRate;
        if($item_obj->salePrice + $item_obj->standardShipRate - $node->field_price['und'][0]['value'] > 0) {
          form_set_error('field_external_url', "Source price is too high");
          return;
        }
        price_check_set_form_value($form, 'field_form_stage', 2, $form_state);
        price_check_set_form_value($form, 'field_store_price', $item_obj->salePrice, $form_state);    
        price_check_set_form_value($form, 'field_shipping_price', $item_obj->standardShipRate, $form_state);    
        price_check_set_form_value($form, 'field_skugrid_local_id', $item_obj->localId, $form_state);    
        if(@$item_obj->upc) price_check_set_form_value($form, 'field_upc', $item_obj->upc, $form_state);
        if(@$item_obj->modelNumber) price_check_set_form_value($form, 'field_mpn', $item_obj->modelNumber, $form_state);
        if(@$item_obj->brandName) price_check_set_form_value($form, 'field_brand', $item_obj->brandName, $form_state);
        $ebay_price = $node->field_price['und'][0]['value'];
        $store_price = $item_obj->salePrice + $item_obj->standardShipRate;
        $fees = $ebay_price * (2.9 + 9)/100 + 0.30;
        $profit = $ebay_price - $store_price - $fees;
        $profit_percent = $profit*100/$store_price;
        if($profit_percent <= 3) {
          if(price_check_is_paying_customer()) {
            drupal_set_message("Profit too low. Consider discarding this item.");
          } else {
            form_set_error('field_external_url', "Profit too low. Discard");
            return;
          }
        }
        if($profit_percent >= 10) {
          $ebay_price = ($store_price * 11 + 3) / 8.81;
          $ebay_price = number_format((float)$ebay_price, 2, '.', '');
        } else {
          $ebay_price -= 1;
        }
        price_check_set_form_value($form, 'field_price', $ebay_price, $form_state); 
      } else {
        if(is_array($item_obj)) {
          watchdog("problem with object", print_r(array('line/file' => __LINE__ ."/" . __FILE__ , 'item_obj' => $item_obj), true));
        }
        form_set_error('field_external_url', "Invalid URL");
      }
      return;
    }
    if($stage == 2) {
      price_check_set_form_value($form, 'field_form_stage', 3, $form_state); 
    }
    if($stage == 3) {
      //watchdog("image:debug", print_r(func_get_args(), true));
      //there must be at least one image
      $image_count = price_check_count_images($form_state);
      if(!$image_count) {
        form_set_error('field_ds_genie_pictures', "You must have at least one image.");
        return;
      }
      $image_count = $form_state['input']['image_count'];
      $images = @$form_state['input']['field_picture']['und'];
      if($images) {
        $new_images = array();
        for($i=0;$i<$image_count;$i++) {
          if(@$form_state['input']['image_checkbox' . $i] == 0) $new_images[] = $images[$i];
        }
        $form['field_picture']['#parents'] = array('field_picture');
        form_set_value($form['field_picture'], array(LANGUAGE_NONE => $new_images), $form_state);
      }
      price_check_set_form_value($form, 'field_complete', 1, $form_state); 
      price_check_set_form_value($form, 'field_complete_date', time(), $form_state); 
      price_check_set_form_value($form, 'field_form_stage', 4, $form_state); 
      if(price_check_is_paying_customer()) {
        $ebay_accounts = price_check_get_ebay_account_names();
        price_check_set_form_value($form, 'field_suggested_accounts', price_check_encode_field($ebay_accounts), $form_state); 
      }
    }
    if($stage == 4) {
      //drupal_set_message("Debug: stage 4 reached");
      //if(price_check_is_paying_customer()) header("Location: /"); exit(0);
    }
    return;
  }
  if($node->type == 'item_template' && ($user->uid == 5 || $user->uid == 168) && @$node->field_locked_for_review['und'][0]['value'] == 1) {
    //print_r($form_state['values']); exit(0);
    //watchdog('test node edit owner', $user->uid);
    $subscribers = price_check_decode_field(@$node->field_suggested_accounts['und']);
    $uid = price_check_get_uid_from_ebay_account($subscribers[0]);
    $account = user_load($uid);
    $node->uid = $uid;
    $node->name = $account->name;
    //node_submit($node);
    $form_state['values']['name'] = $account->name;
    $form_state['values']['uid'] = $uid;
    //watchdog('node edit owner', $account->name);
  }
  if($node->type == 'gift_card') {
    $form['title']['#parents'] = array('title'); 
    form_set_value($form['title'],trim($node->title), $form_state);
  }
  if($node->type == 'item'  && (@$node->field_cron_ignore['und'][0]['value'] == 0)) {
    $url = $node->field_url['und'][0]['url'];    
    //is kmart or walmart item?
    if(strpos($url, 'mygofer.com') !== false) {
      //kmart
      $raw_data = file_get_contents($url);
      preg_match_all("/{\"l(.*)};/m", $raw_data, $results);
      $data = json_decode("{\"" . $results[1][0] . "}");
      if(!isset($data->levels)) {
        form_set_error("Invalid Item URL");
        return;
      }
      preg_match_all("/src=\"(http:\/\/c.shld.net.*)\"/m", $raw_data, $results);
      $thumb = $results[1][0];
      $item = array();
      if($data->itemPrice == 'N/A') {
        //unavailable
        $item = array('stock' => 'Not Available', 'salePrice' => 0, );
      } else $item = array('stock' => 'Available', 'salePrice' => round($data->itemPrice - $data->itemDiscount, 2), );
      $item['name'] = $data->productDescription;
      $item['shortDescription'] = $data->productDescription;
      $item['sku'] = $data->itemNumber;
      $sku = $data->itemNumber;
      $item['thumbnailImage'] = $thumb;
      $obj = (object) $item;
    } else {
      $sku = array_pop(explode("/", $url));
      $json = file_get_contents("http://walmartlabs.api.mashery.com/v1/items/$sku?format=json&apiKey=z58e678x39qen55m2jbqqpmt");
      $obj = json_decode($json);
      if($obj->itemId != $sku) {
        form_set_error("Invalid Item URL");
        return;
      }
    } 
    $form['field_store_title']['#parents'] = array('field_store_title'); 
    form_set_value($form['field_store_title'], array(LANGUAGE_NONE => array(0 => array('value' => $obj->name))), $form_state);
    $form['body']['#parents'] = array('body'); 
    form_set_value($form['body'], array(LANGUAGE_NONE => array(0 => array('value' => $obj->shortDescription))), $form_state);
    $form['field_store_price']['#parents'] = array('field_store_price'); 
    form_set_value($form['field_store_price'], array(LANGUAGE_NONE => array(0 => array('value' => $obj->salePrice))), $form_state);
    $form['field_store_image']['#parents'] = array('field_store_image'); 
    form_set_value($form['field_store_image'], array(LANGUAGE_NONE => array(0 => array('value' => $obj->thumbnailImage))), $form_state);
    $form['field_store_status']['#parents'] = array('field_store_status'); 
    form_set_value($form['field_store_status'], array(LANGUAGE_NONE => array(0 => array('value' => $obj->stock))), $form_state);
    $form['field_sku']['#parents'] = array('field_sku'); 
    form_set_value($form['field_sku'], array(LANGUAGE_NONE => array(0 => array('value' => $sku))), $form_state);
    $form['field_last_checked']['#parents'] = array('field_last_checked'); 
    form_set_value($form['field_last_checked'], array(LANGUAGE_NONE => array(0 => array('value' => time()))), $form_state);
  } 
  if($node->type == "order") {
    if(!empty($node->field_customer_price['und'][0]['value']) && !empty($node->field_order_price['und'][0]['value'])) {
      if(empty($node->field_profit['und'][0]['value'])) {
        $form['field_profit']['#parents'] = array('field_profit'); 
        form_set_value($form['field_profit'], array(LANGUAGE_NONE => array(0 => array('value' => $node->field_customer_price['und'][0]['value'] * 0.881 -  $node->field_order_price['und'][0]['value'] - 0.30))), $form_state);   
      }
    }
  }
  if($node->type == "order" && ($node->field_order_status_select['und'][0]['value'] == 'Product Ordered, waiting on tracking')) {
    if(@$node->field_tracking_number['und'][0]['value']) {
      //we have tracking data
      $carrier = @$node->field_shipper['und'][0]['value'];
      $order_id = @$node->field_ebay_order_id['und'][0]['value'];
      $code = @$node->field_tracking_number['und'][0]['value'];
      $auth_code = price_check_get_ebay_auth_code(@$node->field_ebay_account['und'][0]['value']);
      $result = price_check_tracking_submit($order_id, $code, $carrier, $auth_code);
      if($result!='Success') {
        form_set_error('field_tracking_number', 'There was a problem uploading tracking. Verify tracking number and shipper.');
      } else {
        drupal_set_message('Tracking uploaded for order: ' . $node->field_store_order_number['und'][0]['value']);
        $form['field_tracking_number']['#parents'] = array('field_tracking_number'); 
        form_set_value($form['field_tracking_number'], array(LANGUAGE_NONE => array(0 => array('value' => 'Tracking uploaded'))), $form_state);   
      }
    }
  }
  if($node->type == "order" && ($node->field_order_status_select['und'][0]['value'] == 'Product Ordered, waiting on tracking')) {
    return;
    //need to check each gift card
    $gift_cards = price_check_get_gift_cards($node);
    $errors = array();
    foreach($gift_cards as $gift_card) {
      $gift_card_node = price_check_get_gift_card($gift_card);
      //exists, balance, locked
      if(!$gift_card_node) { 
        $errors['items'][] = "$gift_card not found in the system.";
        continue;
      }
      if($gift_card_node->field_gc_balance['und'][0]['value'] == 0) {
        $errors['items'][] = "$gift_card has zero balance.";
      }
      if(isset($gift_card_node->field_gc_locked_by['und'][0]) && $gift_card_node->field_gc_locked_by['und'][0]['value'] > "") {
        $errors['items'][] = "$gift_card is locked.";
      }
    }
    if(count($errors) > 0 ) form_set_error('', theme('item_list', $errors));
  }
}

function price_check_menu_alter(&$items) {
  $items['node/%node/delete']['context'] = MENU_CONTEXT_INLINE | MENU_CONTEXT_PAGE;
}

function price_check_form_alter(&$form, &$form_state, $form_id) {
  global $user;
  
  if($form_id == 'item_template_node_form' && @$_GET['check_ebay']) {
    $node = $form['#node'];
    $subscribers = price_check_decode_field(@$node->field_subscribers['und']);
    if($subscribers) {
      $raw_subscriber = $subscribers[0];
      $listing = price_check_get_subscriber_from_string($raw_subscriber);
      $ebay_id = $listing['ebay_id'];
      header("Location: /edit_ebay/$ebay_id");
      exit(0);
    }
  }
  if($form_id == 'item_template_node_form' && @$_GET['rawform']) {
    if(price_check_is_paying_customer() && @$form['nid']['#value']) {
      $node = $form['#node'];
      $subscribers = price_check_decode_field(@$node->field_subscribers['und']);
      if($subscribers) {
        $form['actions']['update_ebay'] = array(
          '#type'   => 'submit',
          '#value'  => 'Save & Update Ebay',
        );
        $form['actions']['update_ebay']['#submit'] = $form['actions']['submit']['#submit'];
      }
    }
    return;
  }
  if($form_id == 'price_check_get_seller_items_multiple_form') {
    $form['#validate'][] = 'price_check_get_seller_items_multiple_form_validate';
    return;
  }
  if($form_id == 'invoice_node_form' && !$form['nid']['#value']) {
    $form['field_transaction_id']['#access'] = FALSE;
    $form['field_paid']['#access'] = FALSE;
    $form['author']['#access'] = FALSE;
    $form['menu']['#access'] = FALSE;
    $form['revision_information']['#access'] = FALSE;
    $form['path']['#access'] = FALSE;
    $form['options']['#access'] = FALSE;
  }
  if($form_id == 'bot_node_form' && @$form['nid']['#value']) {
    $form['actions']['run_bot'] = array(
        '#type'   => 'submit',
        '#value'  => 'Run Bot',
        '#attributes' => array('onclick' => 'javascript: return confirm("This will run the bot without saving changes. Are you sure?")'),
      );
      $form['actions']['run_bot']['#submit'][] = 'price_check_bot_run_bot';
  }
  if($form_id == 'item_template_node_form') {
    if(price_check_is_paying_customer() && @$form['nid']['#value']) {
      $form['actions']['update_ebay'] = array(
        '#type'   => 'submit',
        '#value'  => 'Save & Update Ebay',
      );
      $form['actions']['update_ebay']['#submit'] = $form['actions']['submit']['#submit'];
      //$form['actions']['update_ebay']['#submit'][] = 'price_check_update_ebay_item_submit';
    }

    //need to get stage and user perm
    $stage = @$form_state['build_info']['args'][0]->field_form_stage['und'][0]['value'];
    $all_fields = array('field_ebay_url',
      'field_external_url',
      'field_price',
      'title',
      'path',
      'body',
      'field_condition',
      'field_picture',
      'field_ds_genie_pictures',
      'field_category',
      'field_store_price',
      'field_upc',
      'field_brand',
      'field_mpn',
      'field_suggested_accounts',
      'field_subscribers',
      'field_store_status',
      'field_item_history',
      'field_cron_ignore',
      'field_last_checked',
      'field_my_item',
      'field_search_response',
      'field_search_results',
      'field_locked_for_review',
      'field_verified',
      'field_seller',
      'field_quantity_sold',
      'field_imported',
      'field_assigned_to',
      'field_complete',
      'field_complete_date',
      'field_form_stage',
      'field_dispatch_time',
      'field_comments',
      'field_supplier_variation_name',
      'field_shipping_price',
    );
    if(!$stage) {
      //check if there is a price to display
      if(@$form_state['build_info']['args'][0]->field_price['und'][0]['value']) {
        $show_fields = array('field_ebay_url', 'field_external_url', 'field_price', 'field_supplier_variation_name');
      } else {
        $show_fields = array('field_ebay_url', 'field_external_url', 'field_supplier_variation_name');
      }
      if(price_check_is_paying_customer()) {
        $show_fields[] = 'field_my_item';
      }
      price_check_form_show_fields($form, $all_fields, $show_fields);
      $images = @$form_state['build_info']['args'][0]->field_picture['und'];
      if($images) {
        foreach($images as $k => $image) {
          $form['markup' . $k] = array(
            '#type' => 'item',
            '#markup' => price_check_make_image($image['value']),
          );
        }
      }
    }
    if($stage) {
      //ajax upload considerations
      $x = debug_backtrace();
      foreach($x as $k => $i) {
        if($i['function'] == 'file_ajax_upload') return;
      }
      $destination = @$_GET['destination'];
      if(!$destination) {
        $nid = $form['nid']['#value'];
        drupal_goto($_GET['q'] , array('query' => array('destination' => "node/$nid/edit")));
      }
    }
    if($stage == 2) {
      $show_fields = array('field_store_price', 'field_upc', 'field_brand', 'field_mpn', 'body', 'field_price', 'field_shipping_price');
      $form['field_price']['#suffix'] = "<div id='profit_placeholder'></div>";
      $form['field_store_price']['#suffix'] = "<script>jQuery('#profit_placeholder').text('Profit: ' + parseFloat((isNaN(parseFloat(jQuery('#edit-field-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-price-und-0-value').val())) - (isNaN(parseFloat(jQuery('#edit-field-store-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-store-price-und-0-value').val())) - (isNaN(parseFloat(jQuery('#edit-field-shipping-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-shipping-price-und-0-value').val())) - ((isNaN(parseFloat(jQuery('#edit-field-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-price-und-0-value').val())) * (2.9 + 9)/100 + 0.30)).toFixed(2));</script>";
      //shoutout bernard hadeed (bernie mac)  
      $form['field_price'][LANGUAGE_NONE][0]['value']['#attributes'] = array('onkeydown' => "javascript:jQuery('#profit_placeholder').text('Profit: ' + parseFloat((isNaN(parseFloat(this.value)) ? 0 : parseFloat(this.value)) - (isNaN(parseFloat(jQuery('#edit-field-store-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-store-price-und-0-value').val())) - (isNaN(parseFloat(jQuery('#edit-field-shipping-price-und-0-value').val())) ? 0 : parseFloat(jQuery('#edit-field-shipping-price-und-0-value').val())) - ((isNaN(parseFloat(this.value)) ? 0 : parseFloat(this.value)) * (2.9 + 9)/100 + 0.30)).toFixed(2));");
      price_check_form_show_fields($form, $all_fields, $show_fields);
    }
    if($stage == 3) {
      $show_fields = array('field_ds_genie_pictures', 'field_picture');
      price_check_form_show_fields($form, $all_fields, $show_fields);
      $images = @$form_state['build_info']['args'][0]->field_picture['und'];
      if($images) {
        $form['image_count'] = array('#type' => 'hidden', '#value' => count($images));
        foreach($images as $k => $image) {
          $form['markup' . $k] = array(
            '#type' => 'item',
            '#markup' => price_check_make_image($image['value']),
          );
          $form['image_checkbox' . $k] = array(
            '#type' => 'checkbox', 
            '#title' => 'remove',
          );
        }
      }
    }
    if($stage == 4) {
      //paying customers assign the item to their stuff
      if(price_check_is_paying_customer()) {
        if(@$form_state['build_info']['args'][0]->field_my_item['und'][0]['value'] == 1) {
          header("Location: /"); exit(0);
        }
        $seller = @$form_state['build_info']['args'][0]->field_seller['und'][0]['value'];
        $nid = @$form_state['build_info']['args'][0]->nid;
        if(!@$seller) return;
        $sql = <<<SQL
select nid from field_data_field_seller fs
join node n on n.nid = fs.entity_id
where uid = :uid and
field_seller_value = :seller and 
nid not in (select entity_id from field_data_field_complete_date) and
nid > :nid
limit 1
SQL;
        $result = db_query($sql, array(':uid' => $user->uid, ':seller' => $seller, ':nid' => $nid));
        $nid = $result->fetchField();
        $node = @node_load($nid);
        if(!$node) {
          header("Location: /"); exit(0);
        }
        header("Location: /node/$nid/edit?destination=node/$nid/edit"); exit(0);return;
      }
      //fetch the next work item
      $seller = @$form_state['build_info']['args'][0]->field_seller['und'][0]['value'];
      $nid = @$form_state['build_info']['args'][0]->nid;
      if(!@$seller) return;
      $sql = <<<SQL
select nid from field_data_field_assigned_to fa 
join field_data_field_seller fs on fs.entity_id = fa.entity_id
join node n on n.nid = fs.entity_id
where field_assigned_to_value = :uid and
field_seller_value = :seller and 
nid not in (select entity_id from field_data_field_complete_date) and
nid > :nid
limit 1
SQL;
      $result = db_query($sql, array(':uid' => $user->uid, ':seller' => $seller, ':nid' => $nid));
      $nid = $result->fetchField();
      $node = @node_load($nid);
      if(!$node) {
        return;
        header("Location: /"); exit(0);
      }
      header("Location: /node/$nid/edit?destination=node/$nid/edit"); exit(0);
    }
    return;
    $images = @$form_state['build_info']['args'][0]->field_picture['und'];
    if($images) {
      foreach($images as $k => $image) {
        $form['markup' . $k] = array(
          '#type' => 'item',
          '#markup' => price_check_make_image($image['value']),
        );
      }
    }
    $url = @$form_state['build_info']['args'][0]->field_external_url['und'][0]['value'];
    if($url) {
      return;
      $goto = "http://bluephoenixtt.com/l.php?url=" . urlencode($url);
      $form['markup-url'] = array(
        '#type' => 'item',
        '#markup' => "<iframe src='$goto' width='100%', height='500'></iframe>",
        '#weight' => 99999,
      );
    }
  }
  if ($form_id == "views_exposed_form" && $form['#id'] == "views-exposed-form-orders-page") {
    global $user; 
    $user_full = user_load($user->uid);
    $form['field_ebay_account_value']['#type'] = "select";
    $form['field_ebay_account_value']['#size'] = null;
    $form['field_ebay_account_value']['#default_value'] = 'All';
    $form['field_ebay_account_value']['#options'] = array(
      'All' => t('<Any>'),
    );
    $form['field_ebay_account_value']['#validated'] = 'TRUE';
    foreach($user_full->field_ebay_accounts['und'] as $field_ebay_account) {
      $ebay_account = $field_ebay_account['value'];
      $form['field_ebay_account_value']['#options'][$ebay_account] = $ebay_account;
    }
  }
  if($form_id == 'user_profile_form') {
    if(@$_GET['ebay_api']) {
      $all_fields = array(
        'field_user_access_token',
        'field_ebay_access_tokens',
        'field_user_full_name',
        'field_user_telephone',
        'field_user_address_1',
        'field_user_address_2',
        'field_user_city',
        'field_user_state',
        'ckeditor',
        'field_user_zip',
        'field_user_country',
        'field_paypal_email',
        'field_ebay_accounts',
        'masquerade',
        'field_use_gc_system',
        'field_affiliate_hash',
        'field_affiliate_code',
        'field_stripe_id',
        'field_subscription_id',
        'field_active_worker',
        'field_minimum_price',
        'field_minimum_quantity',
      );
      $show_fields = array(
        'field_devid',
        'field_appid',
        'field_certid',
        'field_runame',
        'field_ebay_auth_token',
      );
      price_check_form_show_fields($form, $all_fields, $show_fields);
      unset($form['account']);
    }
  }
}

