<?php

function price_check_get_app_name($uid = false) {
  if($uid) {
    $user_full = user_load($uid);
    return $user_full->field_appid['und'][0]['value'];
  }
  global $user;
  if($user->uid == 0)
    $user_full = user_load(1);
  else
    $user_full = price_check_get_current_user();
  if(@$user_full->field_appid['und'][0]['value']) return @$user_full->field_appid['und'][0]['value'];
  
  if(price_check_is_debug()) {
    return "KwesiSte-7512-4456-b240-1a12f8657b5d";
  } else return "KwesiSte-e28a-433b-9a19-9b337f1edb0a";
}

function price_check_get_ebay_headers($auth_code = false) {
  if($auth_code) {
    $sql = <<<EOSQL
select entity_id from field_data_field_ebay_auth_token where field_ebay_auth_token_value = :auth_code
EOSQL;
    $result = db_query($sql, array(':auth_code' => $auth_code));
    $uid = $result->fetchField();
    if($uid) {
      $user_full = user_load($uid);
      $field_devid = @$user_full->field_devid['und'][0]['value'];
      $field_appid = @$user_full->field_appid['und'][0]['value'];
      $field_certid = @$user_full->field_certid['und'][0]['value'];
      if($field_devid && $field_appid && $field_certid) {
        return array(    
          "X-EBAY-API-APP-NAME: $field_appid", 
          "X-EBAY-API-APP-ID: $field_appid", 
          "X-EBAY-API-DEV-NAME: $field_devid", 
          "X-EBAY-API-CERT-NAME: $field_certid",
        );
      }
    } else {
      return array(
        "X-EBAY-API-APP-NAME: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
        "X-EBAY-API-APP-ID: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
        "X-EBAY-API-DEV-NAME: 71fc60d4-7170-4d1b-9b9a-350c95d62ae2", 
        "X-EBAY-API-CERT-NAME: 906dfa95-09bf-498b-9583-c7ba82373471",
      );
    }
  }
  global $user;
  if($user->uid == 0)
    $user_full = user_load(1);
  else
    $user_full = price_check_get_current_user();
  $field_devid = @$user_full->field_devid['und'][0]['value'];
  $field_appid = @$user_full->field_appid['und'][0]['value'];
  $field_certid = @$user_full->field_certid['und'][0]['value'];
  if($field_devid && $field_appid && $field_certid) {
    return array(    
      "X-EBAY-API-APP-NAME: $field_appid", 
      "X-EBAY-API-APP-ID: $field_appid",
      "X-EBAY-API-DEV-NAME: $field_devid", 
      "X-EBAY-API-CERT-NAME: $field_certid",
    );
  } else {
    return array(
      "X-EBAY-API-APP-NAME: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
      "X-EBAY-API-APP-ID: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
      "X-EBAY-API-DEV-NAME: 71fc60d4-7170-4d1b-9b9a-350c95d62ae2", 
      "X-EBAY-API-CERT-NAME: 906dfa95-09bf-498b-9583-c7ba82373471",
    );
  }
  
  if(price_check_is_debug()) {
    return array(    
      "X-EBAY-API-APP-NAME: KwesiSte-7512-4456-b240-1a12f8657b5d", 
      "X-EBAY-API-APP-ID: KwesiSte-7512-4456-b240-1a12f8657b5d", 
      "X-EBAY-API-DEV-NAME: 71fc60d4-7170-4d1b-9b9a-350c95d62ae2", 
      "X-EBAY-API-CERT-NAME: 986aee9f-2f6a-4f4d-a4ca-4e8a69dda1a8",
    );
  } else 
    return array(
      "X-EBAY-API-APP-NAME: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
      "X-EBAY-API-APP-ID: KwesiSte-e28a-433b-9a19-9b337f1edb0a", 
      "X-EBAY-API-DEV-NAME: 71fc60d4-7170-4d1b-9b9a-350c95d62ae2", 
      "X-EBAY-API-CERT-NAME: 906dfa95-09bf-498b-9583-c7ba82373471",
    );
}

function price_check_get_runame() { 
  global $user;
  if($user->uid == 0)
    $user_full = user_load(1);
  else
    $user_full = price_check_get_current_user();
  if(@$user_full->field_runame['und'][0]['value']) return @$user_full->field_runame['und'][0]['value'];
  
  if(price_check_is_debug()) {
    return "Kwesi_Stewart-KwesiSte-7512-4-xaqtdo";
  } else
    return "Kwesi_Stewart-KwesiSte-e28a-4-pdrxxrsxp";
}

function price_check_get_ebay_credentials() {
  global $user;
  if($user->uid == 0)
    $user_full = user_load(1);
  else
    $user_full = price_check_get_current_user();
  $field_devid = @$user_full->field_devid['und'][0]['value'];
  $field_appid = @$user_full->field_appid['und'][0]['value'];
  $field_certid = @$user_full->field_certid['und'][0]['value'];
  if($field_devid && $field_appid && $field_certid) {
    return "<DevId>$field_devid</DevId>" . 
    "<AppId>$field_appid</AppId>" . 
    "<AuthCert>$field_certid</AuthCert>";
  }
  
  if(price_check_is_debug()) {
    return "<DevId>71fc60d4-7170-4d1b-9b9a-350c95d62ae2</DevId>" . 
       "<AppId>KwesiSte-7512-4456-b240-1a12f8657b5d</AppId>" . 
       "<AuthCert>986aee9f-2f6a-4f4d-a4ca-4e8a69dda1a8</AuthCert>";
  } else 
    return "<DevId>71fc60d4-7170-4d1b-9b9a-350c95d62ae2</DevId>" . 
       "<AppId>KwesiSte-e28a-433b-9a19-9b337f1edb0a</AppId>" . 
       "<AuthCert>906dfa95-09bf-498b-9583-c7ba82373471</AuthCert>";
}

function price_check_get_ebay_item_url($item_id) {
  $env = (price_check_is_debug()) ? "sandbox" : "production";
  $item_urls = array("sandbox" => "http://cgi.sandbox.ebay.com/itm/",
      "production" => "http://ebay.com/itm/");
  return $item_urls[$env] . $item_id; 
}

function price_check_get_ebay_service_url($service = 'default') {
  $env = (price_check_is_debug()) ? "sandbox" : "production";
  $service_urls = array(
    "production" => array("default" => "https://api.ebay.com/ws/api.dll",
      "shopping" => "http://open.api.ebay.com/shopping?version=515",
      "finding" => "http://svcs.ebay.com/services/search/FindingService/v1"),
    "sandbox" => array("default" => "https://api.sandbox.ebay.com/ws/api.dll",
      "shopping" => "http://open.api.sandbox.ebay.com/shopping?version=515",
      "finding" => "http://svcs.sandbox.ebay.com/services/search/FindingService/v1",),
  );
  return $service_urls[$env][$service];
}

function price_check_get_ebay_signin_url($session_id) {
  $runame = price_check_get_runame();
  if(price_check_is_debug()) {
    return "https://signin.sandbox.ebay.com/ws/eBayISAPI.dll?SignIn&RUName=$runame&SessID=$session_id";
  } else 
    return "https://signin.ebay.com/ws/eBayISAPI.dll?SignIn&RUName=$runame&SessID=$session_id";
}

function price_check_get_ebay_auth_token() {
  $user_full = price_check_get_current_user();
  $field_ebay_auth_token = @$user_full->field_ebay_auth_token['und'][0]['value'];
  return $field_ebay_auth_token;
}

function price_check_get_ebay_auth_tokens($uid = false) {
  if($uid) {
    $account = user_load($uid);
    $tokens = price_check_decode_field(@$account->field_ebay_auth_token['und']);
    if($tokens) return $tokens;
    return @$account->field_ebay_auth_tokens['und'][0]['value'];
  }
  global $user;
  if($user->uid == 0) {
    $account = user_load(1);
    return array($account->field_ebay_auth_token['und'][0]['value']);
  }
  if($user->uid) {
    $account = user_load($user->uid);
    return price_check_decode_field($account->field_ebay_auth_token['und']);
  }
  $user_full = price_check_get_current_user();
  $auth_tokens = price_check_decode_field($user_full->field_ebay_access_tokens['und']);
  return $auth_tokens;
}

function price_check_get_items_by_keywords($keywords) {
  $auth_codes = price_check_get_ebay_auth_tokens();
  $auth_code = $auth_codes[0];
  if(!$auth_code) {
    //watchdog("queue:auth", 'using def');
    $auth_code = 'AgAAAA**AQAAAA**aAAAAA**O9Z2VQ**nY+sHZ2PrBmdj6wVnY+sEZ2PrA2dj6AHlYqkD5CHoAydj6x9nY+seQ**IZECAA**AAMAAA**srhOmvMD8GXIYHSzKXg6JK7N0g51cy2Z5aLFSVKRD6q//Eu6Zvm43Oc26eh9PUoX3oumUm6QWuHhB4i8B5zIvwsCST2h0mPlwkcgkQwD097sa1xKKZQ4hqzm84Zs+5RE8kLxCDFNRGRNuVomAkQFrA5cpK1wXskNwp3CowlLzCC//9luGA8mOXrCia7IqfSZ+BppNgAPttaUMXreWXOBy5sjEYBp2/wy6JmTHTax01U9v6oPDWyD1TTza+7jPyVHsUsiuOhG0CLmalQLGNLW2F6+MAuDtSsWSe0NmFpTciTpPs6PbOs5VX248U+Wr3rCcPquGJuFdUzT7ln3bLaJEeeVqZUOg06qlyAp2kTfJovPT9kSR+JtbVyWmCt8jxZJx/VdxaVZsirN/XewUInZtdUFkEURozus2Pzl4ls6ig5VfLdD6vX4DwB1Me69qaEGzZXmFVo0FY5h7gdu/JyPHuL8y4OlwIYdpXCLmhwCBhWgCrKSb9ENGbpChHM8yVXHPwZpI+9bOvU/LNOgcMMBxzSyOn6HeO6Jbcn9OR8f70d8QCH2LdKK1nWXn/46+1t6QnLNGd73R8RZVPu5gsbw8nExuHs4rccbmEQiPNRXzlBPmIXm9OI1lqE4x4d5rM1X9IGxB6NfhGSDvkpCkADLJP9xv+28tvzR01lJRe4ZNV+PtxuGrwHtSWFYXXPWz3yTn327qY666ZTp+zDZ+FXvgHVQDbh1x+qgAeIozJZylvdYmgNSoiWoDErLfCfWNouD';
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<findItemsByKeywordsRequest xmlns="http://www.ebay.com/marketplace/search/v1/services">
  <keywords>$keywords</keywords>
  <outputSelector>SellerInfo</outputSelector>
</findItemsByKeywordsRequest>
XMLREQ;
    $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: findItemsByKeywords",
      "X-EBAY-API-SITEID: 0",
      "X-EBAY-SOA-OPERATION-NAME: findItemsByKeywords",
      "X-EBAY-SOA-SECURITY-APPNAME: " . price_check_get_app_name(),
      "X-EBAY-SOA-SERVICE-NAME: FindingService",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url('finding'));
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}

function price_check_get_ebay_item_transactions($ebay_id, $uid = false) {
  $auth_codes = price_check_get_ebay_auth_tokens($uid);
  $auth_code = $auth_codes[0];
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetItemTransactionsRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <ItemID>$ebay_id</ItemID>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</GetItemTransactionsRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetItemTransactions",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers($auth_code));
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  $time = microtime();
  $time_arr = explode(' ', $time);
  $filename = '/mnt/target03/355840/www.dsgenie.com/web/content/' . drupal_get_path('module', 'price_check') . '/data/' . 
      $time_arr[1] . str_replace('.', '', $time_arr[0]) . ".txt";
  $output = array('usr' => price_check_get_current_user(),
    'arg' => func_get_args(),
    'req' => $xmlRequest,
    'res' => $data);
  //file_put_contents($filename, serialize($output));
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data; 
}

function price_check_get_ebay_seller_items($seller, $page, $uid = false) {
  $auth_codes = price_check_get_ebay_auth_tokens($uid);
  $auth_code = $auth_codes[0];
  //TODO: fix
  if(!$auth_code) {
    //watchdog("queue:auth", 'using def');
    $auth_code = 'AgAAAA**AQAAAA**aAAAAA**O9Z2VQ**nY+sHZ2PrBmdj6wVnY+sEZ2PrA2dj6AHlYqkD5CHoAydj6x9nY+seQ**IZECAA**AAMAAA**srhOmvMD8GXIYHSzKXg6JK7N0g51cy2Z5aLFSVKRD6q//Eu6Zvm43Oc26eh9PUoX3oumUm6QWuHhB4i8B5zIvwsCST2h0mPlwkcgkQwD097sa1xKKZQ4hqzm84Zs+5RE8kLxCDFNRGRNuVomAkQFrA5cpK1wXskNwp3CowlLzCC//9luGA8mOXrCia7IqfSZ+BppNgAPttaUMXreWXOBy5sjEYBp2/wy6JmTHTax01U9v6oPDWyD1TTza+7jPyVHsUsiuOhG0CLmalQLGNLW2F6+MAuDtSsWSe0NmFpTciTpPs6PbOs5VX248U+Wr3rCcPquGJuFdUzT7ln3bLaJEeeVqZUOg06qlyAp2kTfJovPT9kSR+JtbVyWmCt8jxZJx/VdxaVZsirN/XewUInZtdUFkEURozus2Pzl4ls6ig5VfLdD6vX4DwB1Me69qaEGzZXmFVo0FY5h7gdu/JyPHuL8y4OlwIYdpXCLmhwCBhWgCrKSb9ENGbpChHM8yVXHPwZpI+9bOvU/LNOgcMMBxzSyOn6HeO6Jbcn9OR8f70d8QCH2LdKK1nWXn/46+1t6QnLNGd73R8RZVPu5gsbw8nExuHs4rccbmEQiPNRXzlBPmIXm9OI1lqE4x4d5rM1X9IGxB6NfhGSDvkpCkADLJP9xv+28tvzR01lJRe4ZNV+PtxuGrwHtSWFYXXPWz3yTn327qY666ZTp+zDZ+FXvgHVQDbh1x+qgAeIozJZylvdYmgNSoiWoDErLfCfWNouD';
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<findItemsAdvancedRequest xmlns="http://www.ebay.com/marketplace/search/v1/services">
  <itemFilter>
    <name>Seller</name>
    <value>$seller</value>
  </itemFilter>  
  <paginationInput>
    <pageNumber>$page</pageNumber>
  </paginationInput>

</findItemsAdvancedRequest>      
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: findItemsAdvanced",
      "X-EBAY-API-SITEID: 0",
      "X-EBAY-SOA-OPERATION-NAME: findItemsAdvanced",
      "X-EBAY-SOA-SECURITY-APPNAME: " . price_check_get_app_name($uid),
      "X-EBAY-SOA-SERVICE-NAME: FindingService",
    ), price_check_get_ebay_headers($auth_code));
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url('finding'));
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  
  $time = microtime();
  $time_arr = explode(' ', $time);
  $filename = '/mnt/target03/355840/www.dsgenie.com/web/content/' . drupal_get_path('module', 'price_check') . '/data/' . 
      $time_arr[1] . str_replace('.', '', $time_arr[0]) . ".txt";
  $output = array('usr' => price_check_get_current_user(),
    'arg' => func_get_args(),
    'req' => $xmlRequest,
    'res' => $data);
  //file_put_contents($filename, serialize($output));
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}

function price_check_get_ebay_seller_items_old($seller, $page) {
  $auth_codes = price_check_get_ebay_auth_tokens();
  $auth_code = $auth_codes[0];
  //TODO: fix
  if(!$auth_code) {
    //watchdog("queue:auth", 'using def');
    $auth_code = 'AgAAAA**AQAAAA**aAAAAA**O9Z2VQ**nY+sHZ2PrBmdj6wVnY+sEZ2PrA2dj6AHlYqkD5CHoAydj6x9nY+seQ**IZECAA**AAMAAA**srhOmvMD8GXIYHSzKXg6JK7N0g51cy2Z5aLFSVKRD6q//Eu6Zvm43Oc26eh9PUoX3oumUm6QWuHhB4i8B5zIvwsCST2h0mPlwkcgkQwD097sa1xKKZQ4hqzm84Zs+5RE8kLxCDFNRGRNuVomAkQFrA5cpK1wXskNwp3CowlLzCC//9luGA8mOXrCia7IqfSZ+BppNgAPttaUMXreWXOBy5sjEYBp2/wy6JmTHTax01U9v6oPDWyD1TTza+7jPyVHsUsiuOhG0CLmalQLGNLW2F6+MAuDtSsWSe0NmFpTciTpPs6PbOs5VX248U+Wr3rCcPquGJuFdUzT7ln3bLaJEeeVqZUOg06qlyAp2kTfJovPT9kSR+JtbVyWmCt8jxZJx/VdxaVZsirN/XewUInZtdUFkEURozus2Pzl4ls6ig5VfLdD6vX4DwB1Me69qaEGzZXmFVo0FY5h7gdu/JyPHuL8y4OlwIYdpXCLmhwCBhWgCrKSb9ENGbpChHM8yVXHPwZpI+9bOvU/LNOgcMMBxzSyOn6HeO6Jbcn9OR8f70d8QCH2LdKK1nWXn/46+1t6QnLNGd73R8RZVPu5gsbw8nExuHs4rccbmEQiPNRXzlBPmIXm9OI1lqE4x4d5rM1X9IGxB6NfhGSDvkpCkADLJP9xv+28tvzR01lJRe4ZNV+PtxuGrwHtSWFYXXPWz3yTn327qY666ZTp+zDZ+FXvgHVQDbh1x+qgAeIozJZylvdYmgNSoiWoDErLfCfWNouD';
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSellerListRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
  <UserID>$seller</UserID>
  <ErrorLanguage>en_US</ErrorLanguage>
  <WarningLevel>High</WarningLevel>
  <DetailLevel>ItemReturnDescription</DetailLevel>
  <EndTimeFrom>2015-05-07T21:59:59.005Z</EndTimeFrom> 
  <EndTimeTo>2015-07-07T21:59:59.005Z</EndTimeTo> 
  <IncludeWatchCount>true</IncludeWatchCount> 
  <IncludeVariations>true</IncludeVariations>
  <Pagination> 
    <EntriesPerPage>50</EntriesPerPage> 
    <PageNumber>$page</PageNumber>
  </Pagination> 
</GetSellerListRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetSellerList",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data; 
}

function price_check_set_ebay_tax_table($ebay_id, $value, $auth_code) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <ItemID>$ebay_id</ItemID>   
    <UseTaxTable>$value</UseTaxTable>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL: revise item tax2', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;     
}

function price_check_get_ebay_session() {
  $runame = price_check_get_runame();
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSessionIDRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RuName>$runame</RuName>
</GetSessionIDRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetSessionID",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data['SessionID'];
}

function price_check_ebay_token_status($token) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetTokenStatusRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$token</eBayAuthToken>
  </RequesterCredentials>
</GetTokenStatusRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetTokenStatus",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data['TokenStatus']['Status'];
}

function price_check_get_ebay_user_id_from_auth_token($auth_code) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSellerTransactionsRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
 <NumberOfDays>30</NumberOfDays>
 <Pagination>
  <PageNumber>1</PageNumber>
 </Pagination>
</GetSellerTransactionsRequest>
XMLREQ;

  $headers = array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
    "X-EBAY-API-CALL-NAME: GetSellerTransactions",
    "X-EBAY-API-SITEID: 0",
  );

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data['Seller']['UserID'];
}

function price_check_get_ebay_item_debug($item_id) {
  $session = $_SESSION['price_session'][0];  
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <!-- Call-specific Input Fields -->
  <IncludeCrossPromotion> boolean </IncludeCrossPromotion>
  <IncludeItemCompatibilityList> boolean </IncludeItemCompatibilityList>
  <IncludeItemSpecifics> boolean </IncludeItemSpecifics>
  <IncludeTaxTable> boolean </IncludeTaxTable>
  <IncludeWatchCount> boolean </IncludeWatchCount>
  <ItemID> ItemIDType (string) </ItemID>
  <SKU> SKUType (string) </SKU>
  <TransactionID> string </TransactionID>
  <VariationSKU> SKUType (string) </VariationSKU>
  <VariationSpecifics> NameValueListArrayType
    <NameValueList> NameValueListType
      <Name> string </Name>
      <Value> string </Value>
      <!-- ... more Value values allowed here ... -->
    </NameValueList>
    <!-- ... more NameValueList nodes allowed here ... -->
  </VariationSpecifics>
  <!-- Standard Input Fields -->
  <DetailLevel> DetailLevelCodeType </DetailLevel>
  <!-- ... more DetailLevel values allowed here ... -->
  <ErrorLanguage> string </ErrorLanguage>
  <MessageID> string </MessageID>
  <OutputSelector> string </OutputSelector>
  <!-- ... more OutputSelector values allowed here ... -->
  <Version> string </Version>
  <WarningLevel> WarningLevelCodeType </WarningLevel>
</GetItemRequest>
XMLREQ;
}

function price_check_get_ebay_item_name_from_id($item_id) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSingleItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <ItemID>$item_id</ItemID>
</GetSingleItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 905",
      "X-EBAY-API-CALL-NAME: GetSingleItem",
      "X-EBAY-API-SITEID: 0",
      "X-EBAY-API-REQUEST-ENCODING: XML",
      "Content-Type: text/xml;charset=UTF-8",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url('shopping'));
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data['Item']['Title'];  
}

function price_check_proc_get_token() {
  $session = $_SESSION['price_session'][0];
  $xmlRequest = <<<XMLREQ
  <?xml version="1.0" encoding="utf-8"?>
  <FetchTokenRequest xmlns="urn:ebay:apis:eBLBaseComponents">
    <Version>613</Version>
     <SessionID>$session</SessionID>
  </FetchTokenRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 613",
      "X-EBAY-API-CALL-NAME: FetchToken",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;
}

function price_check_cancel_ebay_item($ebay_account, $ebay_id) {
  global $user;
  $uid = $user->uid;
  $auth_tokens = price_check_get_ebay_auth_tokens($uid);
  $auth_token = @$auth_tokens[0];
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<EndFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
 <RequesterCredentials>
    <eBayAuthToken>$auth_token</eBayAuthToken>
  </RequesterCredentials>
  <EndingReason>NotAvailable</EndingReason>
  <ItemID>$ebay_id</ItemID>
</EndFixedPriceItemRequest>   
XMLREQ;
  $headers = array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
    "X-EBAY-API-CALL-NAME: EndFixedPriceItem",
    "X-EBAY-API-SITEID: 0",
  );
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  watchdog('EBAY API CALL: cancel item', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data['Ack'] == "Success";
}

function price_check_get_category_info($category) { 
  global $user;
  $uid = $user->uid;
  $auth_tokens = price_check_get_ebay_auth_tokens($uid);
  $auth_token = @$auth_tokens[0];
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetCategoryInfoRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <CategoryID>$category</CategoryID>

</GetCategoryInfoRequest>
XMLREQ;
  
  //print_r($xmlRequest); exit();
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetCategoryInfo",
      "X-EBAY-API-SITEID: 0",
      "Content-Type: text/xml;charset=UTF-8",
      "X-EBAY-API-VERSION:947",
      "X-EBAY-API-REQUEST-ENCODING:XML",
    ), price_check_get_ebay_headers());

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url('shopping'));
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);

  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;
}

function price_check_get_ebay_orders($auth_token, $page = 1) { 
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSellerTransactionsRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_token</eBayAuthToken>
  </RequesterCredentials>
 <NumberOfDays>14</NumberOfDays>
 <Pagination>
  <EntriesPerPage>1</EntriesPerPage>
  <PageNumber>$page</PageNumber>
 </Pagination>
</GetSellerTransactionsRequest>
XMLREQ;

  $headers = array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
    "X-EBAY-API-CALL-NAME: GetSellerTransactions",
    "X-EBAY-API-SITEID: 0",
  );

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  //watchdog("data:req", print_r($data, true));
  return $data;
}

function price_check_tracking_submit($order_id, $tracking_number, $tracking_service, $auth_code) {
  $xmlRequest = <<<XMLREQ
<?xml version='1.0' encoding='utf-8'?>
<CompleteSaleRequest xmlns='urn:ebay:apis:eBLBaseComponents'>
  <WarningLevel>High</WarningLevel>
  <OrderID>$order_id</OrderID>
  <Paid>true</Paid>
  <Shipped>true</Shipped>
  <Shipment>
    <ShipmentTrackingDetails>
      <ShipmentTrackingNumber>$tracking_number</ShipmentTrackingNumber>
      <ShippingCarrierUsed>$tracking_service</ShippingCarrierUsed>
    </ShipmentTrackingDetails>
  </Shipment>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</CompleteSaleRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: CompleteSale",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL: tracking', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  
  curl_close($ch);
  return $data['Ack'];  
}

function price_check_revise_ebay_item_atom($item_template, $item_id, $auth_code, &$context) {
  $pictureUrl = price_check_decode_field(@$item_template->field_picture['und']);
  $pictureUrlXML = "";
  $allPictures = array();
  $rand = time();
  foreach($pictureUrl as $picture) {
    $pictureUrlXML .= "<PictureURL>$picture?$rand</PictureURL>";
    $allPictures[] = $picture;
  }
  $pictureUrl = price_check_decode_picture_field(@$item_template->field_ds_genie_pictures['und']);
  foreach($pictureUrl as $picture) {
    $pictureUrlXML .= "<PictureURL>$picture?$rand</PictureURL>";
    $allPictures[] = $picture;
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <ItemID>$item_id</ItemID>
     <PictureDetails>
      <PictureSource>Vendor</PictureSource>
      $pictureUrlXML
     </PictureDetails>
     <ShippingDetails>        
      <ApplyShippingDiscount>false</ApplyShippingDiscount>
      <PaymentInstructions>
      Return Policy:We have never had a product returned. Every product comes with our 100% satisfaction guarantee. If you are not happy after initial inspection with quality, materials, or workmanship, please contact us for return shipping instructions. All returns, for any reason, (at Seller expense) must be in original, unused condition and packaged in the same packaging with all original tags still attached. Contact us within 3 days of receipt for return information and we will issue an RMA.
      </PaymentInstructions>
      <SalesTax>
      <SalesTaxPercent>0.0</SalesTaxPercent>
      <ShippingIncludedInTax>false</ShippingIncludedInTax>
      </SalesTax>
      <ShippingServiceOptions>
      <ShippingService>Other</ShippingService>
      <ShippingServiceCost currencyID="USD">0.0</ShippingServiceCost>
      <ShippingServiceAdditionalCost currencyID="USD">0.0</ShippingServiceAdditionalCost>
      <ShippingServicePriority>1</ShippingServicePriority>
      <ExpeditedService>false</ExpeditedService>
      <ShippingTimeMin>1</ShippingTimeMin>
      <ShippingTimeMax>10</ShippingTimeMax>
      <FreeShipping>true</FreeShipping>
      </ShippingServiceOptions>
      <ShippingType>Flat</ShippingType>
      <ThirdPartyCheckout>false</ThirdPartyCheckout>
      <ShippingDiscountProfileID>0</ShippingDiscountProfileID>
      <InternationalShippingDiscountProfileID>0</InternationalShippingDiscountProfileID>
      <ExcludeShipToLocation>Alaska/Hawaii</ExcludeShipToLocation>
      <ExcludeShipToLocation>APO/FPO</ExcludeShipToLocation>
      <ExcludeShipToLocation>US Protectorates</ExcludeShipToLocation>
      <ExcludeShipToLocation>Africa</ExcludeShipToLocation>
      <ExcludeShipToLocation>Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>Central America and Caribbean</ExcludeShipToLocation>
      <ExcludeShipToLocation>Europe</ExcludeShipToLocation>
      <ExcludeShipToLocation>Middle East</ExcludeShipToLocation>
      <ExcludeShipToLocation>North America</ExcludeShipToLocation>
      <ExcludeShipToLocation>Oceania</ExcludeShipToLocation>
      <ExcludeShipToLocation>Southeast Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>South America</ExcludeShipToLocation>
      <ExcludeShipToLocation>PO Box</ExcludeShipToLocation>
      <SellerExcludeShipToLocationsPreference>false</SellerExcludeShipToLocationsPreference>
    </ShippingDetails>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
      //print_r($xmlRequest); exit(0);
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  if($data['Ack'] == 'Success') {
    $context['results'][] = "$item_id OK";
    $context['message'] = t('Updated @item_id', array('@item_id' => $item_id));
    return array('success' => true);
  }
  $context['results'][] = "$item_id failed";
  $context['message'] = t('Failed @item_id', array('@item_id' => $item_id));
  $errors = price_check_translate_errors($data['Errors']); 
  return $errors;
}

function price_check_update_ebay_item($item_id, $auth_code, $params) {
  $xml_frag = '';
  if(@$params['p'] != '') $xml_frag .= "<StartPrice>{$params['p']}</StartPrice>";
  if(@$params['q'] != '') $xml_frag .= "<Quantity>{$params['q']}</Quantity>";    
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    $xml_frag
    <ItemID>$item_id</ItemID>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  watchdog('EBAY API CALL: ajaxmod', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
    global $user;
    //if(@$user->uid == 135) print_r($data); exit(0);
  }
  curl_close($ch);
  if($data['Ack'] == 'Success') return array('success' => true);
  return price_check_translate_errors($data['Errors']);  
}

function price_check_ebay_list_item($item_template, $auth_code) {
  if(!$auth_code || $auth_code == '-') return;
  //$title = htmlentities(@$item_template->title);
  //$title = price_check_html_convert_entities(htmlentities(@$item_template->title));
  $title = @$item_template->title;
  $title = htmlentities(@$item_template->title, ENT_QUOTES, 'UTF-8');
  $title = @$item_template->title;
  $title = str_replace("”", '"', $title);
  $title = str_replace("’", "’", $title);


  //$title = @$item_template->title;
  //Item.ApplicationData
  $conditionID = @$item_template->field_condition['und'][0]['value'];
  $user_full = price_check_get_current_user();
  $add_description_override = @$user_full->field_listing_policy['und'][0]['value'];


$add_description = <<<ADDDESC
    
    
    
Payment:

We accept PayPal only. Even if you have a credit card and no PayPal we can easily accept your credit card through paypal.

Shipping & Handling:

Free Shipping within the 48 states!

We only ship within the continental 48 U.S. states.

We do not ship to Alaska, Hawaii, Puerto Rico, Guam, Virgin islands.

We do not ship to APO/FPO/DPO addresses

We do not ship to P.O.BOXES - please provide a physical address!

We do not ship internationally.

Your order will be processed within 1-4 business days once the payment has been received.

Standard shipping typically takes 3-10 business days.

We do not offer combined shipping services

**Your item will be shipped directly from our supplier. We are a clearinghouse for several major retailers. In the unlikely event that the item is sold out or on backorder, we will contact you to see if you want to wait a few days or we will refund 100% of your money … YOUR choice.
 

Return Policy:

We offer a 14 day return policy from the receiving date. Please contact us for a Return Merchandise Authorization (RMA) number.

All items purchased must be return in the same condition as they were delivered in and in original packaging. They can not have been assembled and disassembled.

Items defective upon receipt must be packaged in their retail packaging as if new and returned with a detailed description of the problem.

Return shipping fees are not refundable. Buyer pays shipping both ways. 20% handling/restocking fee for returns - starting at time of sale. Buyers remorse, finding the item on sale or price variation is not a valid reason for return.


Taxes:

Taxes are charged as determined by your State.


Feedback:

Your satisfaction is very important to us! If you have any problem with your order, please contact us and we will do our very best to make you satisfied.

As long as you are happy with your purchase, please leave a positive feedback for us.

We will always leave you a positive feedback in return.

If you have any problem with your purchase, please do not leave a negative feedback. Please Contact us … and your issue will be solved quickly and positively :)


We are a small family-owned business trying to make it, not a huge company that doesn't care … We will ALWAYS do our best for you!
ADDDESC;
if($add_description_override) {
  $add_description = "<br/>" . $add_description_override;
} else {
  $add_description = nl2br($add_description);
}

$javascript = <<<JSDOC
<script language="JavaScript1.2">
function disabletext(e){
return false
}
function reEnable(){
return true
}
//if the browser is IE4+
document.onselectstart=new Function ("return false")
//if the browser is NS6
if (window.sidebar){
document.onmousedown=disabletext
document.onclick=reEnable
}
</script>
JSDOC;
$desc_html = preg_replace('/<script\b[^>]*>(.*?)<\/script>/is', "", @$item_template->body['und'][0]['value']);


$description =  $desc_html . $add_description . $javascript;
$pictureUrl = price_check_decode_field(@$item_template->field_picture['und']);
$pictureUrlXML = "";
$allPictures = array();
foreach($pictureUrl as $picture) {
  $pictureUrlXML .= "<PictureURL>$picture</PictureURL>";
  $allPictures[] = $picture;
}
$pictureUrl = price_check_decode_picture_field(@$item_template->field_ds_genie_pictures['und']);
foreach($pictureUrl as $picture) {
  $pictureUrlXML .= "<PictureURL>$picture</PictureURL>";
  $allPictures[] = $picture;
}
$categoryID = @$item_template->field_category['und'][0]['value'];
//Item.ShippingDetails
$startPrice = @$item_template->field_price['und'][0]['value'];
$location = price_check_get_user_location();
$paypal_email = price_check_get_paypal_email_new($auth_code);
$brand_frag = price_check_get_brand_xml_from_item_template($item_template);
$user_full = price_check_get_current_user();
$qty = @$user_full->field_minimum_quantity['und'][0]['value'];
if(!$qty) $qty = 1;
$dispatch_time = @$item_template->field_dispatch_time['und'][0]['value'];
if(!$dispatch_time) $dispatch_time = 3;
$xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<AddFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <site>US</site>
    $brand_frag
    <UseTaxTable>True</UseTaxTable>
    <Quantity>$qty</Quantity>
    <Title><![CDATA[$title]]></Title>
    <ApplicationData>FN_ZERO</ApplicationData>
    <AutoPay>True</AutoPay>
    <ConditionID>1000</ConditionID>
    <Country>US</Country>
    <Currency>USD</Currency>
    <Description><![CDATA[$description]]></Description>
    <ListingDuration>GTC</ListingDuration>
    <PaymentMethods>PayPal</PaymentMethods>   
    <PayPalEmailAddress>$paypal_email</PayPalEmailAddress>
    <DispatchTimeMax>$dispatch_time</DispatchTimeMax>   
    <PictureDetails>
      <PictureSource>Vendor</PictureSource>
      <GalleryType>Gallery</GalleryType>
      <GalleryURL>{$allPictures[0]}</GalleryURL>
      <PhotoDisplay>PicturePack</PhotoDisplay>
      $pictureUrlXML
    </PictureDetails>
    <PrimaryCategory>
      <CategoryID>$categoryID</CategoryID>
    </PrimaryCategory>
    <Location>$location</Location>
    <ReturnPolicy>
      <RefundOption>MoneyBack</RefundOption>
      <Refund>Money Back</Refund>
      <ReturnsWithinOption>Days_14</ReturnsWithinOption>
      <ReturnsWithin>14 Days</ReturnsWithin>
      <ReturnsAcceptedOption>ReturnsAccepted</ReturnsAcceptedOption>
      <ReturnsAccepted>Returns Accepted</ReturnsAccepted>
      <Description>If you are dissatisfied with your purchase and need to return it you must contact us within 14 days of receipt of the purchase.</Description>
      <ShippingCostPaidByOption>Buyer</ShippingCostPaidByOption>
      <ShippingCostPaidBy>Buyer</ShippingCostPaidBy>
    </ReturnPolicy>
    <ShippingDetails>        
      <ApplyShippingDiscount>false</ApplyShippingDiscount>
      <PaymentInstructions>
      Return Policy:We have never had a product returned. Every product comes with our 100% satisfaction guarantee. If you are not happy after initial inspection with quality, materials, or workmanship, please contact us for return shipping instructions. All returns, for any reason, (at Seller expense) must be in original, unused condition and packaged in the same packaging with all original tags still attached. Contact us within 3 days of receipt for return information and we will issue an RMA.
      </PaymentInstructions>
      <SalesTax>
      <SalesTaxPercent>0.0</SalesTaxPercent>
      <ShippingIncludedInTax>false</ShippingIncludedInTax>
      </SalesTax>
      <ShippingServiceOptions>
      <ShippingService>Other</ShippingService>
      <ShippingServiceCost currencyID="USD">0.0</ShippingServiceCost>
      <ShippingServiceAdditionalCost currencyID="USD">0.0</ShippingServiceAdditionalCost>
      <ShippingServicePriority>1</ShippingServicePriority>
      <ExpeditedService>false</ExpeditedService>
      <ShippingTimeMin>1</ShippingTimeMin>
      <ShippingTimeMax>10</ShippingTimeMax>
      <FreeShipping>true</FreeShipping>
      </ShippingServiceOptions>
      <ShippingType>Flat</ShippingType>
      <ThirdPartyCheckout>false</ThirdPartyCheckout>
      <ShippingDiscountProfileID>0</ShippingDiscountProfileID>
      <InternationalShippingDiscountProfileID>0</InternationalShippingDiscountProfileID>
      <ExcludeShipToLocation>Alaska/Hawaii</ExcludeShipToLocation>
      <ExcludeShipToLocation>APO/FPO</ExcludeShipToLocation>
      <ExcludeShipToLocation>US Protectorates</ExcludeShipToLocation>
      <ExcludeShipToLocation>Africa</ExcludeShipToLocation>
      <ExcludeShipToLocation>Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>Central America and Caribbean</ExcludeShipToLocation>
      <ExcludeShipToLocation>Europe</ExcludeShipToLocation>
      <ExcludeShipToLocation>Middle East</ExcludeShipToLocation>
      <ExcludeShipToLocation>North America</ExcludeShipToLocation>
      <ExcludeShipToLocation>Oceania</ExcludeShipToLocation>
      <ExcludeShipToLocation>Southeast Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>South America</ExcludeShipToLocation>
      <ExcludeShipToLocation>PO Box</ExcludeShipToLocation>
      <SellerExcludeShipToLocationsPreference>false</SellerExcludeShipToLocationsPreference>
    </ShippingDetails>
  <StartPrice currencyID="USD">$startPrice</StartPrice>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</AddFixedPriceItemRequest>
XMLREQ;
      //watchdog('xml:request fixed price', $xmlRequest);
//if($item_template->nid == 72356) {
      //print_r($xmlRequest); exit(0);
  
//}
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: AddFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  watchdog('EBAY API CALL: list item', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  global $user;
  /*if($user->uid == 3785) {
    print_r($data); exit(0);
  }*/
  //watchdog("list item", print_r(array('result' => $data, 'req' => $xmlRequest), true));
  return $data;  
}

function price_check_ebay_revise_listed_item($item_template, $item_id, $auth_code, $debug = false) {
  if(!$auth_code || $auth_code == '-') return;
  $title = @$item_template->title;
  $title = str_replace("”", '"', $title);
  $title = str_replace("’", "’", $title);
  //$desc_html = preg_replace('/<script\b[^>]*>(.*?)<\/script>/is', "", @$item_template->body['und'][0]['value']);
  $desc_html = @$item_template->body['und'][0]['value'];
  $description =  $desc_html;
  $pictureUrl = price_check_decode_field(@$item_template->field_picture['und'], true);
  $pictureUrlXML = "";
  $allPictures = array();
  foreach($pictureUrl as $picture) {
    $pictureUrlXML .= "<PictureURL>$picture</PictureURL>";
    $allPictures[] = $picture;
  }
  $pictureUrl = price_check_decode_picture_field(@$item_template->field_ds_genie_pictures['und']);
  foreach($pictureUrl as $picture) {
    $pictureUrlXML .= "<PictureURL>$picture</PictureURL>";
    $allPictures[] = $picture;
  }
  $startPrice = @$item_template->field_price['und'][0]['value'];
  $brand_frag = price_check_get_brand_xml_from_item_template($item_template);
  $user_full = price_check_get_current_user();
  $dispatch_time = @$item_template->field_dispatch_time['und'][0]['value'];
  if(!$dispatch_time) $dispatch_time = 3;
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    $brand_frag
    <ItemID>$item_id</ItemID>
    <Title><![CDATA[$title]]></Title>
    <Description><![CDATA[$description]]></Description>
    <PictureDetails>
      <GalleryType>Gallery</GalleryType>
      <GalleryURL>{$allPictures[0]}</GalleryURL>
      <PhotoDisplay>PicturePack</PhotoDisplay>
      $pictureUrlXML
    </PictureDetails>
    <DispatchTimeMax>$dispatch_time</DispatchTimeMax>
    <ShippingDetails>        
      <ApplyShippingDiscount>false</ApplyShippingDiscount>
      <PaymentInstructions>
      Return Policy:We have never had a product returned. Every product comes with our 100% satisfaction guarantee. If you are not happy after initial inspection with quality, materials, or workmanship, please contact us for return shipping instructions. All returns, for any reason, (at Seller expense) must be in original, unused condition and packaged in the same packaging with all original tags still attached. Contact us within 3 days of receipt for return information and we will issue an RMA.
      </PaymentInstructions>
      <SalesTax>
      <SalesTaxPercent>0.0</SalesTaxPercent>
      <ShippingIncludedInTax>false</ShippingIncludedInTax>
      </SalesTax>
      <ShippingServiceOptions>
      <ShippingService>Other</ShippingService>
      <ShippingServiceCost currencyID="USD">0.0</ShippingServiceCost>
      <ShippingServiceAdditionalCost currencyID="USD">0.0</ShippingServiceAdditionalCost>
      <ShippingServicePriority>1</ShippingServicePriority>
      <ExpeditedService>false</ExpeditedService>
      <ShippingTimeMin>1</ShippingTimeMin>
      <ShippingTimeMax>10</ShippingTimeMax>
      <FreeShipping>true</FreeShipping>
      </ShippingServiceOptions>
      <ShippingType>Flat</ShippingType>
      <ThirdPartyCheckout>false</ThirdPartyCheckout>
      <ShippingDiscountProfileID>0</ShippingDiscountProfileID>
      <InternationalShippingDiscountProfileID>0</InternationalShippingDiscountProfileID>
      <ExcludeShipToLocation>Alaska/Hawaii</ExcludeShipToLocation>
      <ExcludeShipToLocation>APO/FPO</ExcludeShipToLocation>
      <ExcludeShipToLocation>US Protectorates</ExcludeShipToLocation>
      <ExcludeShipToLocation>Africa</ExcludeShipToLocation>
      <ExcludeShipToLocation>Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>Central America and Caribbean</ExcludeShipToLocation>
      <ExcludeShipToLocation>Europe</ExcludeShipToLocation>
      <ExcludeShipToLocation>Middle East</ExcludeShipToLocation>
      <ExcludeShipToLocation>North America</ExcludeShipToLocation>
      <ExcludeShipToLocation>Oceania</ExcludeShipToLocation>
      <ExcludeShipToLocation>Southeast Asia</ExcludeShipToLocation>
      <ExcludeShipToLocation>South America</ExcludeShipToLocation>
      <ExcludeShipToLocation>PO Box</ExcludeShipToLocation>
      <SellerExcludeShipToLocationsPreference>false</SellerExcludeShipToLocationsPreference>
    </ShippingDetails>          
    <StartPrice currencyID="USD">$startPrice</StartPrice>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  $debug = true;
  if($debug) {
    $output = array('req' => $xmlRequest, 'res' => $data);
    watchdog("debug: revise item node", print_r($output, true));
  }
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;  
}

function price_check_check_ebay_token_status($token) {
if(!$token || $token == '-') return;
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetTokenStatusRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$token</eBayAuthToken>
  </RequesterCredentials>
</GetTokenStatusRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetTokenStatus",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;
} 

function price_check_exec_ebay_xml($xmlRequest, $service_url, $headers) {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $service_url);
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;
}

function price_check_get_headers($xml_call) {
  return array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
    "X-EBAY-API-CALL-NAME: $xml_call",
    "X-EBAY-API-SITEID: 0",
  );
}

function price_check_get_paypal_email_new($auth_code) {
  $user_full = price_check_get_current_user();
  $email = @$user_full->field_paypal_email['und'][0]['value'];
  if($email) return $email;
  if(price_check_is_debug()) return "kwesistewart-facilitator@kaizenproject.net";
  $email = price_check_get_paypal_email($auth_code);
  if($email) return $email;
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetUserPreferencesRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
 <ShowSellerPaymentPreferences>True</ShowSellerPaymentPreferences>
</GetUserPreferencesRequest>
XMLREQ;

  $headers = price_check_get_headers("GetUserPreferences");
  $data = price_check_exec_ebay_xml($xmlRequest, price_check_get_ebay_service_url(), $headers);
  return @$data['SellerPaymentPreferences']['DefaultPayPalEmailAddress'];
}

function price_check_get_paypal_email($auth_code) {
  if(price_check_is_debug()) return "kwesistewart-facilitator@kaizenproject.net";
  //return price_check_get_paypal_email_new($auth_code);
  //print_r(price_check_get_paypal_email_new($auth_code)); exit(0);
  
  //$user_full = price_check_get_current_user();
  //$email = @$user_full->field_paypal_email['und'][0]['value'];
  //print_r($email); exit(0);
  //if($email) return $email;
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetSellerTransactionsRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
 <NumberOfDays>30</NumberOfDays>
 <Pagination>
  <PageNumber>1</PageNumber>
 </Pagination>
</GetSellerTransactionsRequest>
XMLREQ;

  $headers = array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
    "X-EBAY-API-CALL-NAME: GetSellerTransactions",
    "X-EBAY-API-SITEID: 0",
  );

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  $transactions = @$data['TransactionArray']['Transaction'];
  if(@$transactions['PayPalEmailAddress']) return $transactions['PayPalEmailAddress'];
  if($transactions)
    foreach($transactions as $transaction) return $transaction['PayPalEmailAddress'];
  return false;
}

function price_check_update_user_ebay_item($node) {
  $auth_code = price_check_get_ebay_auth_code($node->field_ebay_account['und'][0]['value']);  
  if(!$auth_code) {
    drupal_get_messages();
    drupal_set_message("Failed to save changes. User not authorized");
    $queue = DrupalQueue::get('price_check_delete_queue'); 
    $queue->createQueue();
    $queue->createItem($node->nid); 
    return;
  }
  $brand_frag = price_check_get_brand_xml_from_ebay_item($node);
  $description = $node->field_ebay_description['und'][0]['value'];
  $ebay_id = $node->field_ebay_item_id['und'][0]['value'];
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    $brand_frag
    <ItemID>$ebay_id</ItemID>
    <AutoPay>True</AutoPay>
    <Description><![CDATA[$description]]></Description>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  if(($data['Ack'] != 'Success') && ($data['Ack'] != 'Warning')) {
    drupal_get_messages();
    drupal_set_message('Failed to save changes to eBay');
  } else {
    drupal_get_messages();
    drupal_set_message('Changes saved to eBay');
  }
  $queue = DrupalQueue::get('price_check_delete_queue'); 
  $queue->createQueue();
  $queue->createItem($node->nid); 
}

function price_check_get_ebay_item_from_id($item_id, $uid = false) {
  $auth_tokens = price_check_get_ebay_auth_tokens($uid);
  $auth_token = @$auth_tokens[0];
  if(!$auth_token) {
    watchdog('debug: no auth token uid->', $uid);
    return false;
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <ItemID>$item_id</ItemID>
  <DetailLevel>ReturnAll</DetailLevel>
  <IncludeItemSpecifics>True</IncludeItemSpecifics>
  <RequesterCredentials>
  <eBayAuthToken>$auth_token</eBayAuthToken>
  </RequesterCredentials>
</GetItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 905",
      "X-EBAY-API-CALL-NAME: GetItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers($auth_token));
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;  
}

function price_check_set_ebay_item_price($ebay_id, $revised_price, $auth_code_or_uid) {
  if(is_numeric($auth_code_or_uid)) {
    $uid = $auth_code_or_uid;
    $auth_tokens = price_check_get_ebay_auth_tokens($uid);
    $auth_code = $auth_tokens[0];
  } else {
    $auth_code = $auth_code_or_uid;
  }
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <StartPrice>$revised_price</StartPrice>
    <ItemID>$ebay_id</ItemID>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers($auth_code));
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  watchdog('EBAY API CALL: revise item price', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}


function price_check_set_ebay_item_quantity($ebay_id, $quantity, $auth_code = false) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <Quantity>$quantity</Quantity>
    <ItemID>$ebay_id</ItemID>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers($auth_code));
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  watchdog('EBAY API CALL: revise item quantity', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}

function price_check_ebay_item_update_tax($ebay_id, $auth_code) {
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseFixedPriceItemRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <Item>
    <AutoPay>True</AutoPay>
    <UseTaxTable>True</UseTaxTable>
    <ItemID>$ebay_id</ItemID>
  </Item>
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseFixedPriceItemRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseFixedPriceItem",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //watchdog('EBAY API CALL: revise item tax', print_r(array('xml'=>$xmlRequest, 'res'=>$data), true));
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}  

function price_check_get_ebay_stored_token() {
  $user_full = price_check_get_current_user();
  return @$user_full->field_ebay_access_tokens['und'][0]['value'];
}

function price_check_get_api_usage($auth_code = false) {
  $data = price_check_get_ebay_info($auth_code);
  $details = @$data['ApiAccessRule'];
  if(!$details) return array('error' => $data);
  foreach ($details as $detail) {
    if($detail['CallName'] == 'ApplicationAggregate') return $detail['DailyUsage'];
  }
}

function price_check_get_ebay_info($auth_token = false) {
  if(!$auth_token) $auth_token = price_check_get_ebay_stored_token();
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<GetApiAccessRulesRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  <RequesterCredentials>
    <eBayAuthToken>$auth_token</eBayAuthToken>
  </RequesterCredentials>
</GetApiAccessRulesRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: GetApiAccessRules",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  //convert the XML result into array
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data;   
}  

function price_check_set_item_quantities($uid, $operations, $quantity) {
  $auth_codes = price_check_get_ebay_auth_tokens($uid);
  $auth_code = $auth_codes[0];
  $output = array();
  foreach($operations as $ebay_id) {
    $xml = <<< EOXML
<InventoryStatus> InventoryStatusType
  <ItemID>$ebay_id</ItemID>
  <Quantity>$quantity</Quantity>
</InventoryStatus>
EOXML;
    $output[] = $xml;
  }
  $xml_frag = implode(' ', $output);
  $xmlRequest = <<<XMLREQ
<?xml version="1.0" encoding="utf-8"?>
<ReviseInventoryStatusRequest xmlns="urn:ebay:apis:eBLBaseComponents">
  $xml_frag
  <RequesterCredentials>
    <eBayAuthToken>$auth_code</eBayAuthToken>
  </RequesterCredentials>
</ReviseInventoryStatusRequest>
XMLREQ;
  $headers = array_merge(
    array("X-EBAY-API-COMPATIBILITY-LEVEL: 967",
      "X-EBAY-API-CALL-NAME: ReviseInventoryStatus",
      "X-EBAY-API-SITEID: 0",
    ), price_check_get_ebay_headers());
  
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, price_check_get_ebay_service_url());
  curl_setopt($ch, CURLOPT_POST, 1);

  curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);

  curl_setopt($ch, CURLOPT_POSTFIELDS,  $xmlRequest);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  $data = curl_exec($ch);
  if($data === false){
    $error = curl_error($ch);
    echo $error; 
    die('error occured');
  }else{
    $data = json_decode(json_encode(simplexml_load_string($data)), true);  
  }
  curl_close($ch);
  return $data; 
}