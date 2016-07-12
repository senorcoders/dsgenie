<?php

  function price_check_get_target_item_from_url($url) {
    $reg = '#(?:http://(?:www\.){0,1}target\.com(?:/.*){0,1}(?:/-/))(.*?)(?:\#|/.*|$)#';
    preg_match($reg, $url, $matches);
    $raw_data = @file_get_contents($url);
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();
      
      $priceBlock = $doc->getElementById('price_main');
      $price = $xp->evaluate('string(.//*[@class="offerPrice"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price);
      
      $shipsBlock = $doc->getElementById('pdpSingleButtonContainer');
      $shipping = trim($xp->evaluate('string(.//p[@class="freeShippingPromo"])', $shipsBlock ));
      $obj ->shipping = $shipping;
    
      $descriBlock = $doc->getElementById('item-overview');
      $desc =  trim($xp->evaluate('string(.//span[@itemprop="description"])', $descriBlock));
      $description =  trim($xp->evaluate('string(.//ul[@class="normal-list"])', $descriBlock));
      $obj ->description = $desc.$description;

      $stockBlock = $doc->getElementById('pdpSingleButtonContainer');
      $stock = $xp->evaluate('string(.//*[@class="shipping"])', $stockBlock);
      $obj->stock = "Not Available";
       if (strpos($stock, 'Out of Stock') == FALSE) 
        $obj->stock = "Available";
       
       $obj->itemId = $matches[1];
    return $obj;
  }

  function price_check_get_bestbuy_item_from_url($url){
    $reg = '#(?:http://(?:www\.){0,1}bestbuy\.com(?:/.*)(?:/.*)(?:&skuId=)(.*))#';
    preg_match($reg, $url, $matches);
    $options = array(
          'http'=>array(
                  'method'=>"GET",
                  'header'=>"Accept-language: en\r\n" .
                  "Cookie: foo=bar\r\n" .  // check function.stream-context-create on php.net
                  "User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:20.0) Gecko/20100101 Firefox/20.0"
                          )
    );
    $context = stream_context_create($options); 
    $raw_data = file_get_contents($url, false, $context);
    
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();

      $priceBlock = $doc->getElementById('price');
      $price = $xp->evaluate('string(.//*[@class="item-price"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price);
    
      $shipsBlock = $doc->getElementById('price');
      $shipping = trim($xp->evaluate('string(.//span[@class="free-shipping-sub-message"])', $shipsBlock ));
      //print_r($shipping);
      if($shipping == "")
        $obj ->shipping = "Free Shipping";
      else
        $obj ->shipping = $shipping;
    
      $descriBlock = $doc->getElementById('overview-tab');
      $description =  trim($xp->evaluate('string(.//div[@itemprop="description"])', $descriBlock));
      $obj ->description = $description;

      $stockBlock = $doc->getElementById('price');
      $stock = trim($xp->evaluate('string(.//*[@class="sale-puck"])', $stockBlock ));
        if($stock !== "On Sale")  
          $obj->stock = "No Available";
        else 
          $obj->stock = "Available";
    
      $obj->itemId = $matches[1];
    
    return $obj;
  }
  
  function price_check_get_wayfair_item_from_url($url){
    $reg = '#(?:http:\/\/(?:www\.){0,1}wayfair\.com\/)(.*).html#';
    preg_match($reg, $url, $matches);
    $raw_data = @file_get_contents($url);
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();

      $price = $xp->evaluate('string(.//*[@data-id="dynamic-sku-price"])');
      if($price == "")
      {
        $price = $xp->evaluate('string(.//*[@class="xxltext wf_accenttext product_price_wrap emphasis fl"])');
        $obj->salePrice = str_replace("$", "", $price);
      }
      else
        $obj->salePrice = str_replace("$", "", $price);
      
      $shipsBlock = $xp ->evaluate('string(.//*[@class="ships_free_pricing_block"])');
      if($shipsBlock =="")
      {
        $shipsBlock = $xp ->evaluate('string(.//*[@class="free_shipping_text"])');
        $obj->ships = $shipsBlock;  
      }
      else
        $obj->ships = $shipsBlock;  
    
      $descriBlock = $xp->evaluate('string(.//*[@class="product_section_description"])');
      if($descriBlock == "")
      {
        $descriBlock = $xp ->evaluate('string(.//*[@class="js-block-content pdp_romance_copy truncate"])');
        $obj ->description = $descriBlock;
        $obj->stock = "Not Available";
        $sku = $xp ->evaluate('string(.//*[@class="fr emphasis ltbodytext "])');
        $obj->itemId = str_replace("SKU #:", "", $sku);
      }
      else
      {
        $obj ->description = $descriBlock;
        $obj->stock = "Available";
        $cadena = $matches[1];
        $obj->itemId = substr(strrchr($cadena, "-"), 1);
      }
      return $obj;
  }
  
  function price_check_get_academy_item_from_url($url){
    
    $reg = '#(?:http://(?:www\.){0,1}academy\.com(?:\/shop)(?:\/pdp)\/)(.*)#'; 
    preg_match($reg, $url, $matches); 
    $raw_data = file_get_contents($url); 
    $doc = new DOMDocument(); 
    $doc->recover = true; 
    @$doc->loadHTML($raw_data); 
    $xp = new DOMXPath($doc); 
    $obj = new stdClass();

      $priceBlock = $doc->getElementById('price_new_rebate_wrap'); 
      $price = $xp->evaluate('string(.//*[@class="price"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price); 
      //$priceBlock = $doc->getElementById('was_price'); 
      //$price = $xp->evaluate('string(.//*[@class="price"])', $priceBlock); 
      
      $shipsBlock = $doc->getElementById('Discounts_3076385');
      $shipping = trim($xp->evaluate('string(.//*[@class="promotion ASO_red"])', $shipsBlock ));

      if ($shipping == "")
        $obj ->shipping = "Free Shipping";
      else
        $obj ->shipping = $shipping;
    
      $descriBlock = $doc->getElementById('description');
      $description =  trim($xp->evaluate('string(.//*[@class="description-content"])', $descriBlock));
      $obj ->description = $description;

      $stockBlock = $doc->getElementById('diffs_add_to_cart'); 
      $stock = trim($xp->evaluate('string(.//*[@class="text"])', $stockBlock)); 
      if($stock == "Not Sold Online") 
        $obj->stock = "Not Sold Online"; 
      elseif ($stock == "Currently Not Available Online")
        $obj->stock = "Not Available";
      else
        $obj->stock = "Available";
    
      $idblock = $doc->getElementById('pdp_promotion_area');
      $iditem = $xp->evaluate('string(.//*[@class="sku"])', $idblock);
      $obj->itemId = str_replace("SKU:", "", $iditem);

    return $obj; 
  }
  
  function price_check_get_amazon_item_from_url($url) {
    $reg = '#(?:http://(?:www\.){0,1}amazon\.com(?:/.*){0,1}(?:/dp/|/gp/product/|/exec/obidos/ASIN/))(.*?)(?:/.*|$)#';
    preg_match($reg, $url, $matches);
    $raw_data = @file_get_contents($url);
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();

      $priceBlock = $doc->getElementById('priceBlock');
      $price = $xp->evaluate('string(.//*[@class="priceLarge"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price);

      $shipsBlock = $doc->getElementById('priceblock_ourprice_row');
      $shipping = $xp->evaluate('string(.//td[contains(.,"shipping")])');
      $cade = substr(strrchr($shipping, "+"), 1);
      $cadena1 = strstr($cade, 'shipping', true); 
      if($cadena1 == "")
        $obj ->shipping = "Free Shipping";
      else  
        $obj ->shipping = str_replace("$", "", $cadena1);
    
      $descriBlock = $doc->getElementById('productDescription');
      $description =  trim($xp->evaluate('string(.//div[@class="productDescriptionWrapper"])', $descriBlock));
      $obj ->description = $description;
    
      $obj->stock = "Not Available";
      if($price) $obj->stock = "Available";
    
      $obj->itemId = $matches[1];
    
    return $obj;
  }
  
  function price_check_get_hayneedle_item_from_url($url){
    $options = array(
          'http'=>array(
                  'method'=>"GET",
                  'header'=>"Accept-language: en\r\n" .
                  "Cookie: foo=bar\r\n" .  // check function.stream-context-create on php.net
                  "User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:20.0) Gecko/20100101 Firefox/20.0"
                          )
    );
    $context = stream_context_create($options); 
    $raw_data = file_get_contents($url, false, $context); 
    $doc = new DOMDocument(); 
    $doc->recover = true; 
    @$doc->loadHTML($raw_data); 
    $xp = new DOMXPath($doc); 
    $obj = new stdClass(); 

      $priceBlock = $doc->getElementById('NPP_MainCont'); 
      $price = $xp->evaluate('string(.//*[@itemprop="price"])', $priceBlock);
      if($price=="")
      {
        $lowprice = $xp->evaluate('string(.//*[@ng-bind="price.min|currency"])', $priceBlock);
        $highprice =$xp->evaluate('string(.//*[@itemprop="highPrice"])', $priceBlock);
        $price = $lowprice. "-" . $highprice;
      }
      $obj->salePrice = str_replace("$", "", $price);
    
      $shipsBlock = $doc->getElementById('CRY356_ShippingCont');
      $shipping = trim($xp->evaluate('string(.//*[@ng-bind="data.ship_msg.back_order_msg"])', $shipsBlock ));
      $obj ->shipping = $shipping;
    
      $descriBlock = $doc->getElementById('NPP_MainCont');
      $description =  trim($xp->evaluate('string(.//span[@class="marginTopOnly5px"])', $descriBlock));
      $obj ->description = $description; 
    
      $stockBlock = $doc->getElementById('NPP_MainCont'); 
      $stock = trim($xp->evaluate('string(.//*[@style="font-size: 22px; padding-top: 5px;"])', $stockBlock)); 
      if($stock == "Out Of Stock") 
        $obj->stock = "Not Available"; 
      else 
        $obj->stock = "Available";
    
      $idblock = $doc->getElementById('NPP_MainCont');
      $iditem = $xp->evaluate('string(.//*[@class="standard-style noWrap"])', $idblock);
      $obj->itemId = $iditem;
    
    return $obj; 
  }
  
  function price_check_get_overstock_item_from_url($url){
    $reg = '#(?:http://(?:www\.){0,1}overstock\.com(?:/.*)(?:/.*)/)(.*)/.*#';
    preg_match($reg, $url, $matches);
    $raw_data = file_get_contents($url);
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();
      
      $priceBlock = $doc->getElementById('pricing-container');
      $price = $xp->evaluate('string(.//*[@itemprop="price"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price);
     
      $shipsBlock = $doc->getElementById('shipping');
      $shipping = trim($xp->evaluate('string(.//div[@class="shipping-returns toggle-content"])', $shipsBlock ));
      $cadena1 = strstr($shipping, 'International Shipping'); 
      $obj ->shipping = $cadena1;
    
      $descriBlock = $doc->getElementById('mainContent');
      $description =  trim($xp->evaluate('string(.//div[@class="description toggle-content"])', $descriBlock));
      $obj ->description = $description;
    
      $stockBlock = $doc->getElementById('addToCartForm');
      $stock = trim($xp->evaluate('string(.//*[@class="price-title"])', $stockBlock));
      //print_r($stock);
      if($stock == "Sale:")  
        $obj->stock = "Not Available";
      else 
        $obj->stock = "Available";
    
      $obj->itemId = $matches[1];
    
    return $obj;
  }
  
  function price_check_get_homedepot_item_from_url($url){
    $reg = '#(?:http://(?:www\.){0,1}homedepot\.com(?:/.*)(?:/.*)/)(.*)#';
    preg_match($reg, $url, $matches);
    $raw_data = file_get_contents($url);
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $obj = new stdClass();
    // Pendiente
      $priceBlock = $doc->getElementById('pricingReg');
      $price = $xp->evaluate('string(.//*[@class="pReg"])', $priceBlock);
      $obj->salePrice = str_replace("$", "", $price);
      
      $shipsBlock = $doc->getElementById('bboxShipToMe');
      $shipping = trim($xp->evaluate('string(.//div[@class="bboxfreeShip b"])', $shipsBlock ));
      $obj ->shipping = $shipping;
      
      $descriBlock = $doc->getElementById('product_description');
      $description =  trim($xp->evaluate('string(.//span[@itemprop="description"])', $descriBlock));
      $obj ->description = $description;


      $stock = trim($xp->evaluate('string(.//*[@class="discontinuedItem show"])'));
      if($stock == "Discontinued")  
        $obj->stock = "No Available";
      else 
        $obj->stock = "Available";
      
      $obj->itemId = $matches[1];
    return $obj;
  }
  
  function price_check_get_walmart_item_id_from_url($url){
      $reg = '#(?:http://(?:www\.){0,1}walmart\.com(?:/.*)(?:/.*)/)(.*)#'; 
      preg_match($reg, $url, $matches);
      $pos = strpos($matches[1], '?');
      if($pos === false)
      {
        $id = $matches[1];
        //print_r($id);
      }
      else
      {
        $id = strstr($matches[1], '?', true);
        print_r($id);
      }
        
  }
  function price_check_get_walmart_item_from_url($url) {
    $sku = array_pop(explode("/", $url));
    $sku = array_shift(explode("?", $sku));
    $json = @file_get_contents("http://walmartlabs.api.mashery.com/v1/items/$sku?format=json&apiKey=z58e678x39qen55m2jbqqpmt");
    if($json === false) {
      return false;
    }  
    $obj = json_decode($json);  
    if($obj->itemId != $sku) {
      return false;
    }
    return $obj;
  }
  
  function price_check_get_item_object_from_url($url) {
    $domain = str_ireplace('www.', '', parse_url($url, PHP_URL_HOST));
   // print_r($domain);
    $host_map = array(
        'amazon.com' => 'price_check_get_amazon_item_from_url',
        'walmart.com' => 'price_check_get_walmart_item_from_url',
        'target.com' => 'price_check_get_target_item_from_url',
        'overstock.com' => 'price_check_get_overstock_item_from_url',
        'wayfair.com' => 'price_check_get_wayfair_item_from_url',
        'academy.com' => 'price_check_get_academy_item_from_url',
        'bestbuy.com' => 'price_check_get_bestbuy_item_from_url',
        'homedepot.com' => 'price_check_get_homedepot_item_from_url',
        'hayneedle.com' => 'price_check_get_hayneedle_item_from_url',
      );
    $fn = @$host_map[$domain];
    if($fn) return $fn($url);
    return false;
  }

  function price_check_get_newegg_item_from_url($url){
    
    $reg = '(?:http://(?:www\.){0,1}newegg\.com(?:/.*){0,1}(?:/.*?Item=)(.*)(?:\&.*))';
    preg_match($reg, $url, $matches);
    
    $raw_data = @file_get_contents($url);
    //print_r($raw_data);
    $fichero = 'html1.txt';
    file_put_contents($fichero, $raw_data);
    $archivoTds1=fopen($fichero, "r+");
    $x = 1;
    //$linea = 0;
    $palabra = "product_id";
    while(!feof($archivoTds1))
    {
      $busca = fgets($archivoTds1);
      $post = strpos($busca, $palabra);
      if ($post !== FALSE)
      {
        print_r($busca);
      }
      $x++;
    }
    fclose($archivoTds1);
    //unlink('html1.txt');
    
    $doc = new DOMDocument();
    $doc->recover = true;
    @$doc->loadHTML($raw_data);
    $xp = new DOMXPath($doc);
    $priceBlock = $doc->getElementById('bodyArea');
    //print_r($priceBlock);
    $price = $xp->evaluate('string(.//*[@itemprop="price"])', $priceBlock);
    $obj = new stdClass();
    $obj->salePrice = str_replace("$", "", $price);
    $stockBlock = $doc->getElementById('synopsis');
    $stock = trim($xp->evaluate('string(.//*[@class="note atnIconP icnAlert"])'));
    
    //print_r($stockBlock);
    if($stock == "OUT OF STOCK.")  
      $obj->stock = "No Available";
    else 
      $obj->stock = "Available";
    $obj->itemId = $matches[1];
    return $obj;
  }
  
  $urls_raw = <<<URLS
						free shipping

http://www.bestbuy.com/site/beats-by-dr-dre-solo-2-on-ear-headphones-blush-rose/9928271.p?id=1219456343208&skuId=9928271
http://www.bestbuy.com/site/samsung-gear-fit-fitness-watch-with-heart-rate-monitor-black/5291009.p?id=1219118572659&skuId=5291009
http://www.bestbuy.com/site/logitech-living-room-k410-wireless-keyboard-black/9359361.p?id=1219405818512&skuId=9359361

						shipping

http://www.bestbuy.com/site/black-rose-pa-digipak-cd/8921049.p?id=3398555&skuId=8921049
http://www.bestbuy.com/site/rachael-ray-4-piece-dinner-plate-set/9680543.p?id=1219494700756&skuId=9680543
http://www.bestbuy.com/site/apple-ipad-air-2-wi-fi-64gb-gold/3315023.p?id=1219090215027&skuId=3315023

						out of stock

http://www.bestbuy.com/site/new-orleans-soul-60s-watch-records-cd-various/4268866.p?id=186868&skuId=4268866
http://www.bestbuy.com/site/watch-how-the-people-dancing-holiday-vinyl/22763593.p?id=2824578&skuId=22763593
http://www.bestbuy.com/site/the-sims-3-plus-showtime-expansion-pack/1308225195.p?id=mp1308225195&skuId=1308225195

						Free shippin

http://www.wayfair.com/Wayfair-Custom-Upholstery-Carly-Loveseat-CSTM1147-CSTM1147.html
http://www.wayfair.com/Areaware-Alarm-Docking-Station-JDCDB2-ARW1190.html
http://www.wayfair.com/WOLF-Heritage-Watch-Storage-Boxes-5-Piece-Watch-Box-99506-WQI1051.html

						Shiping

http://www.wayfair.com/WOLF-Heritage-Watch-Storage-Boxes-2-Piece-Travel-Watch-Box-99504-WQI1049.html
http://www.wayfair.com/Handpresso-Handpresso-Unbreakable-Outdoor-Espresso-Cup-HPCUPS-HDP1001.html
http://www.wayfair.com/Unified-Marine-Female-Shut-Off-Valve-50052441-QYM1504.html

						Out of Stock
http://www.wayfair.com/daily-sales/p/JDS-Personalized-Gifts-GC1082~JMSI1213~E22983.html
http://www.wayfair.com/daily-sales/p/Zipcode-Design-ZIPC1529~ZIPC1529~E21817.html
http://www.wayfair.com/daily-sales/p/WOLF-99507~WQI1052~E22342.html

						Free shippinhttp://blog.dsgenie.com/wp-content/uploads/2015/08/logo2.jpeg

						Shiping
http://www.academy.com/shop/pdp/skechers-mens-supreme-bosnia-thong-sandals?repChildCatid=1455296
http://www.academy.com/shop/pdp/nike-mens-florida-state-university-loyalty-t-shirt?repChildCatid=1198211
http://www.academy.com/shop/pdp/bcg-mens-deceit-running-shoes?repChildCatid=1131708

						Out of Stock
http://www.academy.com/shop/pdp/taurus-1911-45-acp-pistol?repChildCatid=764504
http://www.academy.com/shop/pdp/taurus-7-round-magazine-for-709-pistols?repChildCatid=871212
http://www.academy.com/shop/pdp/game-winner-camo-waterfowl-backpack?repChildCatid=1171229

						Free Shippin

http://www.amazon.com/Apple-iPhone-Verizon-Cellphone-Black/dp/B0074R0Z3O/ref=sr_1_2?ie=UTF8&qid=1439413109&sr=8-2&keywords=iphone
http://www.amazon.com/Bluetooth-WristWatch-Android-Smartphone-Samsung/dp/B00KW70WDG/ref=sr_1_6?ie=UTF8&qid=1439993948&sr=8-6&keywords=watch
http://www.amazon.com/Nike-Mens-Running-Black-White/dp/B00K7I0Q2W/ref=sr_fsl_cat_softlines_brand_shv_2?s=apparel&ie=UTF8&qid=1439994043&sr=1-2-fed_softlines_strip_1&nodeID=10445813011&keywords=shoes

						Shipping

http://www.amazon.com/AUTOMATIC-DOUBLE-DISPLAY-STORAGE-BATTERY/dp/B00AZPN548/ref=sr_1_3?ie=UTF8&qid=1439996427&sr=8-3&keywords=rolex+box
http://www.amazon.com/Wristwatch-Showcase-Display-Storage-Holders/dp/B00VN9K0XE/ref=sr_1_12?ie=UTF8&qid=1439996427&sr=8-12&keywords=rolex+box
http://www.amazon.com/BOCS-Xtender-Whole-Home-Entertainment-System/dp/B001FG4HHA/ref=sr_1_1?ie=UTF8&qid=1439997040&sr=8-1&keywords=boc+watch

						Out of Stock

http://www.amazon.com/LG-G4-Black-Leather-Sprint/dp/B00XMGU35Q/ref=sr_1_9?s=wireless&ie=UTF8&qid=1439996903&sr=1-9&keywords=lg+g4
http://www.amazon.com/Blue-Oyster-Cult-Long-Night/dp/B00006L923/ref=sr_1_4?ie=UTF8&qid=1439997040&sr=8-4&keywords=boc+watch
http://www.amazon.com/Splinter-Blacklist-Ultimatum-Collector-SPECIAL-Watch/dp/B00KTO326G/ref=sr_1_103?ie=UTF8&qid=1440174449&sr=8-103&keywords=box+watch


						Shipping and free Shipping

http://www.overstock.com/Electronics/Apple-iPhone-6-16GB-4G-LTE-Unlocked-GSM-Cell-Phone-w-iOS8/9447380/product.html?searchidx=0
http://www.overstock.com/US/es/Jewelry-Watches/Movado-Mens-2100005-Collection-Yellow-Goldplated-Swiss-Quartz-Watch/7972138/product.html?refccid=IUDVCC4GR6IYEH2IGITPZBBHIU&searchidx=4
http://www.overstock.com/Clothing-Shoes/Asics-Mens-Gel-Resolution-5-Black-and-Blue-Tennis-Shoes/9821141/product.html?refccid=P3XAFWGQEOXZYWSERZFH4UTXEM&searchidx=0

						Out of Stock
http://www.overstock.com/Jewelry-Watches/Goldtone-Crystal-Christmas-Stocking-and-Candy-Cane-Brooch/8342088/product.html?refccid=CPF7VI54EMO2X3CF4FERQDZMLU&searchidx=6
http://www.overstock.com/Clothing-Shoes/Hadari-Womens-Crochet-V-neck-Romper/10184727/product.html?refccid=CPF7VI54EMO2X3CF4FERQDZMLU&searchidx=10
http://www.overstock.com/Clothing-Shoes/TOI-ET-MOI-Womens-Salad-01-Peep-toe-Wedge-Sandals/10007903/product.html?refccid=CPF7VI54EMO2X3CF4FERQDZMLU&searchidx=19
URLS;

  $urls = explode("\n", $urls_raw);
  foreach($urls as $url) {
    if(strpos($url, "http") === false) continue;
    $obj = price_check_get_item_object_from_url($url);
    file_put_contents("./results/testresults.txt",  print_r(array('url' => $url, 'obj' => $obj), true), FILE_APPEND);
  }
?>
