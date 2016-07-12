<?php


function price_check_skugrid_get_api_key() {
  return "SkuRemote_dsgeniemarc_268910732956";
}

function price_check_string_to_hex($data) {
  $output = '';
  $data = str_replace('"', "\\\"", $data);
  $parts = str_split($data);
  foreach($parts as $char) {
    $output .= "%" . dechex(ord($char));
  }
  return $output;
}

function price_check_skugrid_add_item($url, $vendor_variant = false) {
  $api_key = price_check_skugrid_get_api_key();
  //$url_encoded = json_encode($url);
  $params = array('vendor_url' => urlencode($url),
      'instant_update' => 1);
  if($vendor_variant) $params['vendor_variant'] = price_check_string_to_hex($vendor_variant);
  $add_item = json_encode($params);
  $request = "http://skugrid.com/remoteApi/index.php?remoteKey=$api_key&addItem=$add_item";
  if(@$_GET['debug']) {
      //print_r($request); exit();
  }
  $json = @file_get_contents($request); 
  if(@$_GET['debug']) {
    //print_r(json_decode($json)); exit();
  }
  return json_decode($json);
}

function price_check_skugrid_get_item($local_id_or_url, $vendor_variant = false) {
  $api_key = price_check_skugrid_get_api_key();
  if(is_numeric($local_id_or_url)) {
    $request = "http://skugrid.com/remoteApi/index.php?remoteKey=$api_key&getItem=$local_id_or_url";
    $json = @file_get_contents($request);
    return json_decode($json);
  } else {
    $item_req = price_check_skugrid_add_item($local_id_or_url, $vendor_variant);
    if(@$_GET['debug']) {
      //print_r($item_req); exit(0);
      //print_r(price_check_skugrid_get_item($item_req->local_id)); exit();
    }
    return price_check_skugrid_get_item($item_req->local_id);
  }
}

function price_check_skugrid_get_item_object($vendor_url, $vendor_variant = false) {
  $skugrid_obj_raw = price_check_skugrid_get_item($vendor_url, $vendor_variant);
  $skugrid_obj = $skugrid_obj_raw->Item[0];
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
  return $obj;
}

function price_check_skugrid_get_csv() {
  $api_key = price_check_skugrid_get_api_key();
  $request = "http://skugrid.com/remoteApi/index.php?remoteKey=$api_key&exportCsv=true";
  $json = file_get_contents($request);
  $skugrid_obj = json_decode($json);
  return file_get_contents($skugrid_obj->csv); 
}

function price_check_skugrid_csv_to_object($headers, $rowdata) {
  //watchdog('debug:skugrid_csv', print_r(array($headers, $rowdata), true));
  $skugrid_obj = new stdClass();
  foreach($headers as $k => $v) {
    $skugrid_obj->$v = @$rowdata[$k];
  }
  $obj = new stdClass();
  $obj->localId = @$skugrid_obj->local_id;
  $obj->title = @$skugrid_obj->vendor_title;
  $obj->image = @$skugrid_obj->vendor_image;
  $obj->vendorVariant = @$skugrid_obj->vendor_variant;
  $obj->salePrice = @$skugrid_obj->vendor_price;
  $obj->standardShipRate = @$skugrid_obj->vendor_shipping;
  if(@$skugrid_obj->vendor_stock != 1)  
    $obj->stock = "Not Available";
  else 
    $obj->stock = "Available";
  $obj->itemId = @$skugrid_obj->vendor_sku;
  //price_check_cron_print("csv obj " . print_r(array($headers, $rowdata, $skugrid_obj, $obj), true));
  return $obj;
}

/*stdClass Object
(
    [Item] => Array
        (
            [0] => stdClass Object
                (
                    [local_id] => 2326053
                    [remote_id] => 11422747
                    [compare_id] => 11423207
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
                    [last_active] => 2016-04-15 14:06:00
                    [deleted] => 0
                    [date_added] => 2016-04-15 16:51:56
                    [vendor] => WALMART
                    [vendor_url] => http://www.walmart.com/ip/EAGLE-1205BEI-Can-Butt-5-G-Beige/40893284
                    [vendor_sku] => 40893284
                    [vendor_variant] => 
                    [vendor_stock] => 1
                    [vendor_price] => 41.13
                    [vendor_shipping] => 5
                    [vendor_title] => EAGLE 1205BEI Cigarette Receptacle, Beige, Steel, 5 Gal.
                    [vendor_image] => http://i5.walmartimages.com/dfw/dce07b8c-11d4/k2-_fe7e4b4b-fe4d-4693-b2fc-162af5a8b6c7.v2.jpg
                    [retail_price] => 0
                    [date_updated] => 2016-04-15 17:06:37
                    [date_update_started] => 0000-00-00 00:00:00
                    [failsafe] => 0
                    [errors] => 0
                    [compare_vendor] => Manual Entry
                    [compare_sku] => 
                    [compare_url] => #14607543909879
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
 * 
 */