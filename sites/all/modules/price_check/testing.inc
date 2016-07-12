<?php

include_once DRUPAL_ROOT . '/sites/all/modules/price_check/pages.dev.inc';

function price_check_get_details_atom($ebay_id, &$context) {
  $item = price_check_get_ebay_item_from_id($ebay_id, 637);
  $output = $item['Item']['ItemID'] . "," .
  $item['Item']['ApplicationData'] . "," .
  $item['Item']['ListingDetails']['ViewItemURL'];
  watchdog("debug: dump 637", $output);
  $context['message'] = $output;
}

function price_check_get_details_finished() {
  
}


function price_check_testing_walmart_comparison_atom($url, &$context) {
  $obj1 = price_check_get_walmart_item_from_url($url);
  $obj2 = price_check_get_walmart_item_from_url_new($url);
  sleep(2);
  $price1 = price_check_get_price_with_shipping($obj1);
  $price2 = price_check_get_price_with_shipping($obj2);
  $stock1 = $obj1->stock;
  $stock2 = $obj2->stock;
  if((price_check_get_price_with_shipping($obj1) != price_check_get_price_with_shipping($obj2)) ||
      ($obj1->stock != $obj2->stock)) {
    
    $context['message'] = 'failed ' . $obj->itemId;
    $context['results'][] = "\"fail\",$price1,$price2,$stock1,$stock2,$url";
    return;
  }
  $context['results'][] = "\"pass\",$price1,$price2,$stock1,$stock2,$url";
  
}
  
  /*begin snippet: finished*/
  function price_check_testing_walmart_comparison_finished($success, $results, $operations) {
    print implode("\r\n", $results); exit(0);
  }
  
function price_check_menu_rebuild() {
  drupal_flush_all_caches();
  return "done";
}





function price_check_mass_update_orders() {
  /*variable_set("cron_seq", 0); exit(0);
  file_put_contents("/var/www/html/sites/default/logs/123/test.txt", "test"); exit();
  mkdir("/var/www/html/sites/default/logs/123"); exit();*/
  
  /*
  $url = "http://www.walmart.com/ip/Suddenly-Slim-By-Catalina-Women-s-Plus-Size-Retro-Shirred-Halter-Slimming-Swimdress/47088407";
  //$url = "http://www.walmart.com/ip/Mainstays-Black-Metal-Arm-Futon-with-6-Mattress-Multiple-Colors/45732681";
  $url = "http://www.amazon.com/Helens-Pinkmartini-Colors-Sneakers-shoelace/dp/B017978YK4?SubscriptionId=AKIAJZ5RM3DQXSWELRIQ&tag=affiliate-20&linkCode=xm2&camp=2025&creative=165953&creativeASIN=B017978YK4";
  $url = "http://www.amazon.com/Helens-Pinkmartini-Colors-Sports-Sneakers/dp/B01AQNRL3O/ref=sr_1_1?s=apparel&ie=UTF8&qid=1460698223&sr=1-1&nodeID=7141123011&keywords=shoes";
  $obj = price_check_get_item_object_from_url($url, true); 
  print_r($obj);
  exit(0);
  
  */
  
  /*
  $queuenames = array('price_check_walmart_queue',
    'price_check_walmart_queue_atom',
    'price_check_general_queue',
    'price_check_skugrid_queue',
    'price_check_skugrid_queue_atom');
  $results = array();
  foreach($queuenames as $queuename) {
    $results[$queuename] = DrupalQueue::get($queuename)->numberOfItems(); 
  }
  $output = "<table><tr><th>name</th><th>count</th></tr>";
  foreach($results as $k => $v) {
    $output .= "<tr><td>$k</td><td>$v</td></tr>";
  }
  $output .= "</table>";
  print $output;
  exit(0);
  
  
  $skugrid_queue = DrupalQueue::get('price_check_skugrid_queue'); 
  $queue_count = $skugrid_queue->numberOfItems();
  
  
  
  
  watchdog("debug:skugrid force", time());
  $file = fopen("/var/www/html/sites/default/skugrid.csv","r");
  $headers = fgetcsv($file);
  $queue = DrupalQueue::get('price_check_skugrid_queue'); 
  $queue->createQueue();
  while(!feof($file))
  {
    $row = fgetcsv($file);
    $obj = price_check_skugrid_csv_to_object($headers, $row);
    //print_r($obj); exit();
    $queue->createItem($obj); 
  }
  fclose($file);
  exit();
  
  
  
  
  print_r(price_check_skugrid_get_csv()); exit();
  
  
  $item = "http://www.walmart.com/ip/45804384";
  $item = "http://www.walmart.com/ip/47377647?findingMethod=wpa&wpa_qs=SuaEVXgwZpMw36XCgejH4uDEZFU03QqU6B9iOEd_ZsU&tgtp=2&cmp=-1&pt=hp&adgrp=-1&plmt=__plmt__&bkt=__bkt__&pgid=0&adUid=__adUid__&adiuuid=adb3373b-1197-4c16-8fe5-f0f7fbfa08d6&adpgm=hl&pltfm=desktop";
  $item = "http://www.walmart.com/ip/MuscleTech-Pro-Series-Myobuild-4X-Amino-BCAA-Fruit-Punch-Blast-Dietary-Supplement-11.71-oz/47375937?action=product_interest&action_type=title&item_id=47375937&placement_id=irs-2-m2&strategy=PWVAV&visitor_id&category=&client_guid=19deece5-fc0e-4ad3-ab54-2df6737d7cd2&customer_id_enc&config_id=2&parent_item_id=47377647&parent_anchor_item_id=47377647&guid=3038c477-9992-4817-a02b-f7fcaa90735b&bucket_id=irsbucketdefault&beacon_version=1.0.1&findingMethod=p13n";
  $item = "http://www.walmart.com/ip/12516992";
  $item = "http://www.walmart.com/ip/10452571";
  $item = "http://www.walmart.com/ip/Children-s-Benadryl-Cherry-flavored-Allergy-4-fl-oz/10849236?action=product_interest&action_type=title&item_id=10849236&placement_id=irs-2-m3&strategy=PWBAB&visitor_id&category=&client_guid=ca149a95-3c67-4168-905e-239a5b29ea54&customer_id_enc&config_id=2&parent_item_id=10452571&parent_anchor_item_id=10452571&guid=18cc7ca7-9531-4444-b5f5-d4465894e0e8&bucket_id=irsbucketdefault&beacon_version=1.0.1&findingMethod=p13n";
  //$item = "http://www.walmart.com/ip/Children-s-Benadryl-Cherry-flavored-Allergy-4-fl-oz/10849236";
  $item = "http://www.walmart.com/ip/Pediacare-Children-s-Grape-Flavor-Fever-Reducer-Pain-Reliever-Plus-Multi-Sympton-Cold-4-oz/17248577?action=product_interest&action_type=title&item_id=17248577&placement_id=irs-2-m3&strategy=PWBAB&visitor_id&category=&client_guid=d46bb616-9476-430f-8367-f142a82dedda&customer_id_enc&config_id=2&parent_item_id=10849236&parent_anchor_item_id=10849236&guid=56b14864-a0ae-4b19-88d0-bd0e17bc7f78&bucket_id=irsbucketdefault&beacon_version=1.0.1&findingMethod=p13n";
  $item = "http://www.walmart.com/ip/Pediacare-Children-s-Grape-Flavor-Fever-Reducer-Pain-Reliever-Plus-Multi-Sympton-Cold-4-oz/17248577";
  
  print_r(price_check_skugrid_get_item(urlencode($item))); exit();
  
  
  $json = price_check_skugrid_add_item();
  $obj = json_decode($json);
  print_r($obj); 
  
  $local_id = $obj->local_id;
  
  
  
  
  exit(0);
  //price_check_create_walmart_queue();


  //menu_rebuild();
  //return "done";
  //menu_rebuild();
  //return;

//price_check_create_walmart_queue(); exit(0);
  //print time(); exit(0);
  
  */
  require_once ('/var/www/html/jpgraph/jpgraph.php');
  require_once ('/var/www/html/jpgraph/jpgraph_line.php');
  require_once ('/var/www/html/jpgraph/jpgraph_bar.php');

  
  
  $sql = <<<EOSQL
select
count(field_last_checked_value) c, 
  round((unix_timestamp() - cast(field_last_checked_value as unsigned))/60) l      
-- nid, cast(field_last_checked_value as unsigned), from_unixtime(cast(field_last_checked_value as unsigned), '%Y %D %M %H:%i:%s') 
from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_data_field_external_url fu on fu.entity_id = nid
left join field_data_field_last_checked fc on  fc.entity_id = n.nid
where name ='Paying Customer' and type = 'item_template'
and nid in (select entity_id from field_data_field_subscribers)
and cast(field_last_checked_value as unsigned) > 0
group by round((unix_timestamp() - cast(field_last_checked_value as unsigned))/60)
order by round((unix_timestamp() - cast(field_last_checked_value as unsigned))/60) asc;
EOSQL;
  $results = db_query($sql);
  
  $data = array();
  foreach($results as $row) {
    $data[$row->l] = $row->c;
  }
  

  
  $xmax = max(array_keys($data));
  $xmin = min(array_keys($data));
  
  for($x=$xmin;$x<$xmax;$x++) {
    if(!@$data[$x]) $data[$x] = 0;
  }
  ksort($data);
  $ydata = array_values($data);
  
  //print print_r($ydata, true); exit();
  
  $sql = <<<EOSQL
select count(field_last_checked_1_value) c, 
  round((unix_timestamp() - cast(field_last_checked_1_value as unsigned))/60) l from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_revision_field_external_url fu on fu.entity_id = nid
left join field_revision_field_last_checked_1 fc on  fc.entity_id = n.nid
where name ='Paying Customer' and type = 'item_template'
and nid in (select entity_id from field_revision_field_subscribers)
and nid not in (select entity_id from field_revision_field_data where field_data_value like 'invalid%')
and field_external_url_value like '%walmart%'
group by round((unix_timestamp() - cast(field_last_checked_1_value as unsigned))/60)
order by round((unix_timestamp() - cast(field_last_checked_1_value as unsigned))/60) asc;
EOSQL;
  /*$results = db_query($sql);
  $data = array();
  foreach($results as $row) {
    $data[$row->l] = $row->c;
  }
  $xmax = max(array_keys($data));
  $xmin = min(array_keys($data));
  for($x=$xmin;$x<$xmax;$x++) {
    if(!@$data[$x]) $data[$x] = 0;
  }
  ksort($data);
  $ydata2 = array_values($data);
  
  */
  
  
  
  
  
  $graph = new Graph(2000, 1000);
  $graph->SetScale('intlin');
  $graph->SetShadow();
  $graph->SetMargin(40,30,20,40);
  $bplot = new BarPlot($ydata);
  
  //$graph->Add($bplot);
  
  //$bplot2 = new BarPlot($ydata2);
  
  //$graph->Add($bplot2);
  
  
  //$gbplot = new GroupBarPlot (array($bplot ,$bplot2)); 
  $gbplot = new GroupBarPlot (array($bplot)); 
  $graph->Add($gbplot);
  
  
  
  $bplot->SetFillColor('orange');
  //$bplot2->SetFillColor('red');
  
  $graph->title->Set('DsGenie Tracking Status');
  $graph->xaxis->title->Set('Offset * 60');
  $graph->yaxis->title->Set('Listings');
  $graph->title->SetFont(FF_FONT1,FS_BOLD);
  $graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
  $graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);
  //exec('rm /var/www/html/testxx.png');
  $graph->Stroke('/var/www/html/sites/default/files/testxx.png');
  header("Location: /sites/default/files/testxx.png");
  //$graph->Stroke();
  exit(0);
  return;
  
  
  
  
  /*($current = variable_get('mail_system', array('default-system' => 'DefaultMailSystem'));
  $addition = array('mymodule' => 'PriceCheckMailSystem');
  variable_set('mail_system', array_merge($current, $addition));
  return "done";*/
  
  
  price_check_prepare_email_diagnostic();
  return "done";
  menu_rebuild();
  return;
  
  
  price_check_fix_items_custom();
  return;
  
  
  
  
  $obj = price_check_get_ebay_item_from_id('311538632703', 633);
  print_r($obj); exit(0);
  
  
  
  
  $obj[] = price_check_get_item_object_from_url("http://www.wayfair.com/Adeptus-25.8-Roll-Cart-C0008-ADPT1002.html");
  $obj[] = price_check_get_item_object_from_url("http://www.wayfair.com/Upholstered-Headboard-THRE1904-THRE1904.html");
  print_r($obj); exit(0);
  
  
  $obj = price_check_get_item_object_from_url("http://www.walmart.com/ip/Seville-Classics-UltraHD-Rolling-Storage-Cabinet-with-Drawers/22228868");
  print_r($obj); exit(0);
  
  
  $sql = <<<EOSQL
select fc.entity_id, field_external_url_value
from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_data_field_external_url fu on fu.entity_id = nid
join field_data_field_last_checked fc on  fc.entity_id = fu.entity_id
where name ='Paying Customer' and type = 'item_template'
and 
(fu.entity_id in (select entity_id from field_data_field_subscribers)
or
fc.entity_id in (select entity_id from field_data_field_suggested_accounts))
and field_external_url_value like '%walmart.com%'
order by cast(field_last_checked_value as unsigned) asc
limit 90
EOSQL;
  $results = db_query($sql);
  $operations = array(); 
  foreach($results as $row) {
    $url = $row->field_external_url_value;
    $operations[] = array(
      'price_check_testing_walmart_comparison_atom',
      array($url),
    );
  }
  $batch = array(
    'title' => 'Walmart comparison',
    'operations' => $operations,
    'finished' => 'price_check_testing_walmart_comparison_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/testing.inc',
  );
  batch_set($batch);
  batch_process('/');  
  return;
  
  $obj = price_check_get_item_object_from_url("http://www.wayfair.com/Weston-Game-Processing-Knife-Set-83-7001-W-WEN1080.html");
  print_r($obj); exit();
  
  $id = $_GET['id'];
  $obj = price_check_get_walmart_item_from_url_new("/$id");
  print_r($obj); exit();
  
  $data = <<<EOD
151647364291
161800583655
161754703995
161754735050
161798760110
151713095025
161733586734
161733623223
151766946710
161776096413
151647348416
161760236763
161897083351
151647377010
151889976872
161804506886
161718350010
161667550272
161882841487
161800561642
161820424499
161778693258
161786609087
161798993129
151806340375
151743949199
161513649459
151758613599
151770919107
151785274928
151648843900
161710005198
161739447802
151736294985
151738428751
161739656666
161786129940
151513262528
161748237815
151719624678
151814539284
151770961070
151645347268
161581654218
151728722170
161824579097
161667610141
161517413325
151602946331
161726231930
161783734431
151643422355
161709568952
161786404841
161817515019
161713680222
161670555739
151729644225
151883522149
151766292637
161710014448
161746088250
161786429187
151762069468
161817796111
161766165228
151766998587
151738581384
151650314905
161649820430
151650321196
161673446423
161739432831
151709403077
151575739423
161728519344
161782628804
151735000073
151729816524
151729707819
161814949149
161732276052
161731804018
161747517339
151742748395
151758646518
151806070496
161780644264
161795344270
161900552733
151763709850
161766165228
161739432831
161728519344
EOD;
  $item_ids = explode("\n", $data);
  $operations = array();
  foreach($item_ids as $ebay_id) {
    $operations[] = array(
      'price_check_get_details_atom',
      array($ebay_id),
    );
  }
  $batch = array(
    'title' => 'Ebay Data Dump',
    'operations' => $operations,
    'finished' => 'price_check_get_details_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/testing.inc',
  );
  batch_set($batch);
  batch_process('/');
  return;
  
  $data =<<<EOD
16013	221832592053
16740	321813814057
17928	321816334134
17986	321816641117
23969	321846305576
26268	221847904957
28813	321832140397
30141	221854157939
30161	321833748560
30591	321834594039
59192	221866231463
62650	321854224596
63611	221872267549
63623	221873166595
64138	321854225096
66254	221879069611
68046	221882686687
68346	321862417887
68611	321863275149
68657	321864244244
68658	321864244244
68672	221885014569
68673	321864506896
68678	221885069059
70026	321866716020
70028	321866715899
72523	321870595264
75295	221894683632
81981	321882543783
82182	321882755272
82286	221903703193
82300	321883477281
82301	221903835937
86545	221912407645
86781	221909495509
87260	321889729931
87377	321889900251
87517	321890111641
90609	321897038304
90665	321900498991
92515	221914555897
93524	321895626649
94716	321897039135
99956	321901482549
105504	221931503000
110553	221931516247
123624	221941379568
123627	221941379652
125652	221942092009
127096	221943152594
129979	321924752267
129985	321924756950
69142	221886898607
68619	321863324867
26249	321827451683
88084	321892197206
94112	321896423957
82283	221950503710
125661	321921819856
65553	321856607332
72511	321870526473
63622	221873166293
92574	321894424655
23922	221846160128
65551	221876935612
92565	221914556241
68620	321863330593
87681	321890315557
23094	321823951888
26655	321828515808
51264	221861793092
25025	321819925629
73427	221895070109
87672	321890303389
87259	321889723977
93989	221916316910
123615	321922038955
51222	221861780560
63600	221872240743
63606	221872251359
92493	221914554962
92572	321894424254
18783	321816641117
16942	221834954672
87252	321889715412
92684	221914750290
68674	221885026054
125654	321921840582
93518	321895612208
30606	321834826475
17976	321816612783
93973	221916308029
93977	221916307798
87671	221910504025
94726	221828785253
82489	321884124047
63595	321852472154
EOD;
  $work = explode("\n", $data);
  foreach($work as $workitem) {
    $w = explode("\t", $workitem);
    $nid = $w[0];
    $ebay_id = $w[1];
    $ebay_account = "legrandeproducts";
    $subscriber = "$ebay_account|$ebay_id";
    $node = node_load($nid);
    $node->field_subscribers['und'][0]['value'] = $subscriber;
    node_save($node);
    watchdog("debug: relinked item", "$nid|$subscriber");
  }
  exit(0);
  
  
  print_r(price_check_get_ebay_seller_items('upfront0812', 1)); exit(0);
  
  
  price_check_walmart_filter(); exit(0);
  
  
  $nid = @$_GET['nid'];
  $node = node_load($nid);
  $ext_url = $node->field_ebay_url['und'][0]['value'];
  $ebay_item = price_check_get_ebay_item_from_url($ext_url);
  $pictures = price_check_force_json_array($ebay_item['Item']['PictureDetails']['PictureURL']);
  $picture_output = array();
  //print_r($pictures); exit(0);
  if($pictures) {
    foreach($pictures as $picture) {
      $picture_output[] = array('value' => price_check_save_ebay_picture($picture));
    }
    $node->field_picture['und'] = $picture_output;
  }
  node_save($node); print_r("finished"); exit(0);
  $item = price_check_get_ebay_item_from_id("151739439653");
  print_r($item); exit(0);
  
  return price_check_auto_work();
  
  $data = <<<DATA
9815
15651
15980
15981
15982
16697
18011
18073
18087
18091
18497
18498
18499
18723
18723
18772
18776
18994
19003
19283
19285
19300
19318
19336
19345
19391
19393
19397
19399
19433
19446
19511
20171
20192
20203
20504
21517
21518
21520
21523
22020
22077
22106
22108
DATA;
  return;
  $results = explode("\n", $data);
  foreach($results as $nid) {
    $node = node_load($nid);
    if(!$node) continue;
    $node->field_suggested_accounts['und'][0]['value'] = 'moneysaver85';
    $node->field_subscribers['und'] = array();
    node_save($node);
  }
  print "done"; exit(0);
  
  
  
  
  $active_workers = price_check_get_active_workers();
  if($worker_count = count($active_workers)) {
    $workload = count($results) / $worker_count;
    $partitions = price_check_partition($results, $worker_count);
    foreach($active_workers as $i => $worker) {
      price_check_assign_work($worker, @$partitions[$i]);
    }
  }
  exit(0);
  
  
  $context = array();
  $item['url'] = "http://www.ebay.com/itm/131574950908";
  price_check_batch_get_seller_items_details_atom($item, $context);
  
  
  exit(0);
  $nid = 60385;
  $node = node_load($nid);
  
  $sql = <<<SQL
select distinct field_ebay_accounts_value from role r
join users_roles ur on ur.rid = r.rid
join users u on u.uid = ur.uid
join field_data_field_ebay_accounts fn on fn.entity_id = u.uid
where r.name = 'Paying Customer'
SQL;
  $results = db_query($sql);
  $blacklist_sellers = array();
  foreach($results as $row) {
    $blacklist_sellers[] = $row->field_ebay_accounts_value;
  }
  $node->field_seller_blacklist['und'] = price_check_encode_field($blacklist_sellers);
  
    $sql = <<<SQL
select distinct field_seller_value from field_data_field_seller where field_seller_value > ''
and field_seller_value not in (select field_ebay_accounts_value from role r
join users_roles ur on ur.rid = r.rid
join users u on u.uid = ur.uid
join field_data_field_ebay_accounts fn on fn.entity_id = u.uid
where r.name = 'Paying Customer')
SQL;
  $results = db_query($sql);
  $sellers = array();
  foreach($results as $row) {
    $sellers[] = $row->field_seller_value;
  }
  $node->field_sellers['und'] = price_check_encode_field($sellers);
  node_save($node);
  
  
  
  
  exit(0);
  $seller = 'renleys';
  $output = price_check_get_ebay_seller_items_new($seller, 1);
  print_r($output); exit(0);
  
  $ebay_id = '200545332927';
  print_r(price_check_get_item_sales_30($ebay_id)); exit(0);
  
  if(@$_SESSION['price_check_fix_amazon_links']) {
    unset($_SESSION['price_check_fix_amazon_links']);
    return 'done';
  }
  $sql = <<<SQL
select fu.entity_id from field_data_field_external_url fu
join field_data_field_store_status fs on fs.entity_id = fu.entity_id
where field_external_url_value  like '%amazon.com%'
and fu.entity_id not in (select entity_id from field_data_field_data where field_data_value = 'invalid')
order by entity_id desc
SQL;
  $results = db_query($sql);
  $operations = array();
  foreach($results as $row) {
    $operations[] = array(
      'price_check_check_amazon_links_atom',
      array($row->entity_id),
    );
  }
  $batch = array(
    'title' => 'Check Amazon Links',
    'operations' => $operations,
    'finished' => 'price_check_fix_amazon_links_finished',
  );
  batch_set($batch);
  batch_process('mass_debug');  
  return;
  $nids = <<<NIDS
55368
55036
54986
55129
54907
54998
54880
55104
55369
NIDS;
  $nid_array = explode("\r\n", $nids);
  foreach($nid_array as $nid) {
    $node = node_load($nid);
    $node->uid = 637;
    $node->field_suggested_accounts['und'][0]['value'] = 'upfront0812';
    node_save($node);
    print "$nid\r\n";
  }
  exit(0);
  node_delete_multiple($nid_array);
  /*foreach($nid_array as $nid) {
    node_delete($nid);
    drupal_set_message("deleted $nid");
  }*/
  return;
  
  
  
  //print_r(price_check_get_overstock_item_from_url("http://www.overstock.com/Home-Garden/Wood-Leg-White-Accent-Chairs-Set-of-2/4429638/product.html")); exit(0);
  //print_r(price_check_get_overstock_item_from_url("http://www.overstock.com/Clothing-Shoes/Out-of-Stock/2936062/product.html")); exit(0);
  
  
  
  return;
  
  
   $n = node_load(6340);
  print_r($n); exit(0);
  
  ini_set('max_execution_time', 300);
  $work = <<<EOD
mrchachastore|251947054402
mrchachastore|251947052555
mrchachastore|251947055090
mrchachastore|261876249478
mrchachastore|261894678043
mrchachastore|261894678920
mrchachastore|251947058426
mrchachastore|251947059176
mrchachastore|261876251236
mrchachastore|261894680255
EOD;
  
  foreach(explode("\n", $work) as $work_item) {
    $subscriber = price_check_get_subscriber_from_string($work_item);
    $auth_code = price_check_get_ebay_auth_code_admin($subscriber['ebay_account']);
    $ebay_id = trim($subscriber['ebay_id']);
    $result = price_check_update_tax_table($ebay_id, $auth_code);
    print "$ebay_id " . $result . "<br />";
    watchdog("DEBUG:Z", "$ebay_id " . $result, array(), WATCHDOG_DEBUG, NULL);
  }
  
  exit(0);
  
  
  $sql = "select * from field_revision_field_picture where field_picture_value like '%ebay%'";
  $result = db_query($sql);
  foreach($result as $record) {
    $pic = price_check_save_ebay_picture($record->field_picture_value);
    $sql2 = "update field_revision_field_picture set field_picture_value = :pic where entity_id = :entity_id and delta = :delta";
    $sql3 = "update field_data_field_picture set field_picture_value = :pic where entity_id = :entity_id and delta = :delta";
    db_query($sql2, array(":pic"=> $pic, ":entity_id"=>$record->entity_id, ":delta" => $record->delta));
    db_query($sql3, array(":pic"=> $pic, ":entity_id"=>$record->entity_id, ":delta" => $record->delta)); 
  }
  
  return ;
  $sql = 'select distinct field_item_name_value from field_revision_field_item_name '.
      "where bundle = 'order' and field_item_name_value like ':%' limit 10";
    $result = db_query($sql);
    foreach($result as $record) { 
      $pieces = explode(" ", $record->field_item_name_value);
      $ebay_id = str_replace(":", "", @$pieces[0]);
      $item_title = price_check_get_item_name_from_ebayid($ebay_id);
      echo $ebay_id . " " . $item_title . "<br />";
      if(@$_GET['save']) {
        $sql2 = "update field_revision_field_item_name set field_item_name_value = :item_title where ".
          "bundle = 'order' and field_item_name_value like :ebay_id";
        db_query($sql2, array(":item_title" => $item_title, ":ebay_id" => '%' . db_like($ebay_id) . '%'));
        echo "(prev (saved))";
      }
    }
    exit(0);
    
  $outp = price_check_fetch_page_proxy("http://www.ebay.com/itm/181573987484");
  
  preg_match_all("/<h1.*>(.*)<\/h1>/i", $outp, $results);
  $title = @$results[1][0];
  
  //<h1 class="it-ttl" itemprop="name" id="itemTitle"><span class="g-hdn">Details about  &nbsp;</span>Adjustable Plastic Safety Gate Pet Child Baby  Mountable Safe Home Stairs NEW</h1>
  print_r($title);exit(0);
  
  $outp = file_get_contents("http://www.ebay.com/sch/m.html?_sop=12&_ssn=classic365&_pgn=2&_skc=50&rt=nc");
  print_r($outp);exit(0);
  
global $user;
$user_full = user_load($user->uid);
print_r($user_full); exit(0);
  
  //price_check_fix_orphaned_fields();
  return;
//update items in orders
  /*$result = db_query("SELECT n.nid FROM {node} n WHERE n.type = 'order'");
  foreach ($result as $record) {
    $node = node_load($record->nid);
    $ebay_item_id = substr($node->field_item_name['und'][0]['value'], 0, 12);
    if(is_numeric($ebay_item_id)) {
      $name = price_check_get_item_name($ebay_item_id);
      if(substr($name,0,1) != ':') {
        $node->field_item_name['und'][0]['value'] = $name;
        node_save($node);
      }
    }
  }
  return;*/



  $happy = <<<EOD
878	9405509699939589293131+	
876	9405509699938110828729+	
875	9405509699937241046002+	
874	9405509699937241046699+	
873	9405509699939589305636-	
870	9405509699938110830432+	
869	9405509699938110830715+	
868	9405509699939589306398+	
866	9405509699937241050009+	
864	9405509699937241078362+	
863	9405509699939589337347+	
862	9405509699937241079055+	
860	9405509699937241079284-	
832	9405509699937231300411	
831	9405509699937231300664	
825	9405509699937231299395	
823	9405509699938101053116	
822	9405509699938101052089+	
820	9405509699938101050207+	
819	9405509699937231294444+	
818	9405509699937231292808+	
817	9405509699937231289198+	
816	9405509699938101042486+	
815	9405509699938101038274+	
EOD;

$changes = array('happyfatherof2'=>$happy);

  
  foreach($changes as $ebay_account => $dataraw) {
    $data = explode("\n", $dataraw);
    foreach($data as $order_number) {
      //print_r($order_number); exit(0);
      $order_status = 'Tracking uploaded';
      if(strpos($order_number, "+") !== false) $order_status = 'Order complete, delivered';
      $node = menu_get_object("node", 1, drupal_lookup_path("source", "content/$ebay_account/" . substr($order_number, 0, 3)));
      $node->field_order_status_select['und'][0]['value'] = $order_status;
      node_save($node);
      print "$order_number \r\n";
    }
  }
  print "done"; exit(0);
}

function price_check_walmart_filter() {
  set_time_limit(0);  
  $sql = <<<SQL
select fu.entity_id, field_external_url_value from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_data_field_external_url fu on fu.entity_id = nid
join field_data_field_last_checked fc on  fc.entity_id = fu.entity_id
where name ='Paying Customer' and type = 'item_template'
and field_external_url_value like '%walmart.com%'
and fc.entity_id in (select entity_id from field_data_field_subscribers)
and fc.entity_id not in (select entity_id from field_data_field_data)
order by cast(field_last_checked_value as unsigned) asc
SQL;
    $results = db_query($sql);
    $c = 0;
    foreach($results as $row) {
      $c++;
      if($c % 5 == 0) sleep(1);
      $nid = $row->entity_id;
      $item = price_check_get_walmart_item_from_url($row->field_external_url_value);
      $node = node_load($nid);
      if($item) {
        $node->field_data['und'][0]['value'] = 'valid_walmart';
      } else {
        $node->field_data['und'][0]['value'] = 'invalid_walmart';
      } 
      node_save($node);
    }
}

function price_check_fix_prices_display() {
  $sql = <<<EOSQL
select fs.entity_id, field_subscribers_value,delta from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_data_field_subscribers fs on fs.entity_id = nid
where name ='Paying Customer' and type = 'item_template'
and length(field_subscribers_value) - length(replace(field_subscribers_value, '|', '')) >= 2
EOSQL;
  $results = db_query($sql);
  $operations = array();
  foreach($results as $row) {
    $operations[] = array(
      'price_check_fix_prices_display_request',
      array(
        $row->entity_id,
        $row->field_subscribers_value,
        $row->delta,
      ),
    );
  }
  $batch = array(
    'title' => 'Updating items',
    'operations' => $operations,
    'finished' => 'price_check_fix_prices_display_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/testing.inc',
  );
  batch_set($batch);
  batch_process('/');
}

function price_check_fix_prices_display_request($nid, $raw_subscriber, $delta, &$context) {
  $subscriber = price_check_get_subscriber_from_string($raw_subscriber);
  $ebay_id = $subscriber['ebay_id'];
  $ebay_item = price_check_get_ebay_item_from_id($ebay_id);
  $ebay_price = $ebay_item['Item']['StartPrice'];
  $node = node_load($nid);
  $node->field_price['und'][0]['value'] = $ebay_price;
  $node->field_subscribers['und'][$delta]['value'] = "{$subscriber['ebay_account']}|{$subscriber['ebay_id']}";
  node_save($node);
  $context['message'] = "Saved {$node->title}";
  drupal_set_message("Saved {$node->title}");
}

function price_check_fix_prices_display_finished($success, $results, $operations)  {
}

function price_check_fix_walmart_invalid() {
  $sql = <<<EOSQL
select entity_id from field_data_field_data where field_data_value like 'invalid%'
EOSQL;
  
  $sql = <<<EOSQL
select * from field_data_field_store_price where field_store_price_value = '4.97';
EOSQL;
  
  
  $sql = <<<EOSQL
select fs.entity_id, field_price_value, field_store_price_value from role r
join users_roles ur on r.rid = ur.rid 
join node n on n.uid = ur.uid
join field_data_field_subscribers fs on fs.entity_id = nid
join field_data_field_store_price fp on fp.entity_id = nid
join field_data_field_price fpp on fpp.entity_id = fp.entity_id 
where name ='Paying Customer' and type = 'item_template'
and field_price_value < field_store_price_value * 1.15;
EOSQL;
  
  $results = db_query($sql);
  $operations = array();
  foreach($results as $row) {
    $operations[] = array(
      'price_check_fix_walmart_invalid_request',
      array($row->entity_id),
    );
  }
  $batch = array(
    'title' => 'Updating items',
    'operations' => $operations,
    'finished' => 'price_check_fix_walmart_invalid_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/testing.inc',
  );
  batch_set($batch);
  batch_process('/');
}

function price_check_fix_walmart_invalid_request($nid, &$context) {
  $node = node_load($nid);
  /*if($node) {
    $ext_url = $node->field_external_url['und'][0]['value'];
    $item_obj = price_check_get_item_object_from_url($ext_url);
    if(@$item_obj->salePrice) {
      $node->field_data['und'][0]['value'] ='';
      $node->field_store_price['und'][0]['value'] = price_check_get_price_with_shipping($item_obj);
      node_save($node);
      $context['message'] = "$nid fixed";
      return;
    }
  }*/
  if($node) {
    $subscriber = price_check_get_subscriber_from_string($node->field_subscribers['und'][0]['value']);
    $ebay_id = $subscriber['ebay_id'];
    if($ebay_id) {
      $item = price_check_get_ebay_item_from_id($ebay_id);
      $ebay_price = $ebay_item['Item']['StartPrice'];
      $node->field_price['und'][0]['value'] = $ebay_price;
      node_save($node);
      $context['message'] = "$nid fixed";
      return;
    }
  }
  $context['message'] = "$nid skipped";
  drupal_set_message("$nid skipped");
}

function price_check_fix_walmart_invalid_finished($success, $results, $operations)  {
  
}

function price_check_relink_ebay_items($start_page = 0) {
  //get all the seller items
  global $user;
  $uid = $user->uid;
  $ebay_accounts = price_check_get_ebay_account_names();
  $seller = $ebay_accounts[0];
  if($start_page) {
    $page = $start_page;
  } else {
    $page = 1;
  }
  $response = price_check_get_ebay_seller_items($seller, $page, $uid);
  //print_r($response); exit(0);
  $items = price_check_force_json_array($response['searchResult']['item']);
  $all_items = array();
  foreach($items as $item) {
    $all_items[] = array('title' => $item['title'], 'itemId' => $item['itemId']);
  }
  
  /*if(!$start_page) {
    $numpages = @$response['paginationOutput']['totalPages'];
    if($numpages > 1) for($page=2;$page<=$numpages;$i++) {
      $response = price_check_get_ebay_seller_items($seller, $page, $uid);
      $items = price_check_force_json_array($response['searchResult']['item']);
      foreach($items as $item) {
        $all_items[] = array('title' => $item['title'], 'itemId' => $item['itemId']);
      }
    }
  }*/
  
  $operations = array();
  foreach($all_items as $item) {
    $operations[] = array(
      'price_check_relink_ebay_items_atom',
      array($seller,$item),
    );
  }
  $batch = array(
    'title' => 'Relink eBay items',
    'operations' => $operations,
    'finished' => 'price_check_relink_ebay_items_finished',
    'file' => drupal_get_path('module', 'price_check')  . '/testing.inc',
  );
  batch_set($batch);
  $numpages = @$response['paginationOutput']['totalPages'];
  $page++;
  if($page > $numpages) {
    batch_process("/");  
  } else {
    batch_process("relink_ebay/$page");  
  }
  return;
}

function price_check_relink_ebay_items_atom($seller, $item, &$context) {
  global $user;
  $uid = $user->uid;
  $title = $item['title'];
  $sql = "select nid from node where title = :title and uid = :uid";
  $results = db_query($sql, array('title' => $title, 'uid' => $uid));
  $itemId = $item['itemId'];
  
  $subscriber_raw = "$seller|$itemId";
  $count = 0;
  foreach($results as $row) {
    $nid = $row->nid;
    $node = node_load($nid);
    $node->field_subscribers['und'][0]['value'] = $subscriber_raw;
    node_save($node);
    $context['message'] = "$title found in DSGenie and relinked";
    $count++;
  }
  if(!$count) {
    $context['message'] = "$title not found in DSGenie";
    //drupal_set_message("$title not found in DSGenie");
    watchdog("debug: relink", "$uid:$seller:$title:$itemId");
    return;
  }
}

function price_check_relink_ebay_items_finished($success, $results, $operations)  {
  
}

function price_check_relink_ebay_items_output() {
  
}

function price_check_admin_relink_ebay_items() {
  
}