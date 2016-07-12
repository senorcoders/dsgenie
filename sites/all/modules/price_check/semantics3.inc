<?php

/*
API Key:	SEM32AAE1ACC500F259529D0D66E8C930120
Secret:	NTEwNzZhN2M4Mzk4MWUxNDM5NTZmNTU2ZTQ2NDY5MjU
 * 
 */

function price_check_call_semantics3() {
  require_once('/mnt/target03/355840/www.dsgenie.com/web/content/semantics-php/Semantics3.php');
  $key = 'SEM32AAE1ACC500F259529D0D66E8C930120';
  $secret = 'NTEwNzZhN2M4Mzk4MWUxNDM5NTZmNTU2ZTQ2NDY5MjU';
  $requestor = new Semantics3_Products($key,$secret);
  return $requestor;
} 

function price_check_get_semantics3_product($upc) {
  $response = price_check_get_cached_semantics3_product($upc);
  if($response) return $response;
  $requestor->products_field("upc", $upc);
  $response = $requestor->get_products();
  $requestor->clear_query();
  $timestamp = time();
  $node = new stdClass();
  $node->title = $upc;
  $node->type = "upc_query";
  node_object_prepare($node); 
  $node->language = LANGUAGE_NONE; 
  $node->status = 1; 
  $node->promote = 0; 
  $node->comment = 0; 
  $node->field_last_requested['und'][0]['value'] = $timestamp ;
  $node->field_last_checked['und'][0]['value'] = $timestamp;
  $node->field_response['und'][0]['value'] = $response;
  $node = node_submit($node); 
  node_save($node);
  return json_decode($response, true);
}

function price_check_get_cached_semantics3_product($upc) {
  //need to create node for it
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'upc_query')
    ->propertyCondition('title', $upc, '=')
    ->propertyCondition('status', 1);
  $result = $query->execute();
  if(isset($result['node'])) {
    foreach($result['node'] as $nid => $v) {
      $node = node_load($nid);
      if($node) {
        $timestamp = time();
        $node->field_last_requested['und'][] = array('value' => $timestamp);
        node_save($node);
        return $node->field_response['und'][0]['value'];
      }
    }
  }
  return false;
 }
