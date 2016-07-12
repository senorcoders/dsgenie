<?php

/**
 * Random user agent creator
 * @since Sep 4, 2011
 * @version 1.0
 * @link http://360percents.com/
 * @author Luka Pušić <pusic93@gmail.com>
 */
/**
 * Sources:
 * http://en.wikipedia.org/wiki/Usage_share_of_web_browsers#Summary_table
 * http://statowl.com/operating_system_market_share_by_os_version.php
 */
function price_check_chooseRandomBrowserAndOS() {
    $frequencies = array(
        34 => array(
            89 => array('chrome', 'win'),
            9 => array('chrome', 'mac'),
            2 => array('chrome', 'lin')
        ),
        32 => array(
            100 => array('iexplorer', 'win')
        ),
        25 => array(
            83 => array('firefox', 'win'),
            16 => array('firefox', 'mac'),
            1 => array('firefox', 'lin')
        ),
        7 => array(
            95 => array('safari', 'mac'),
            4 => array('safari', 'win'),
            1 => array('safari', 'lin')
        ),
        2 => array(
            91 => array('opera', 'win'),
            6 => array('opera', 'lin'),
            3 => array('opera', 'mac')
        )
    );
    $rand = rand(1, 100);
    $sum = 0;
    foreach ($frequencies as $freq => $osFreqs) {
        $sum += $freq;
        if ($rand <= $sum) {
            $rand = rand(1, 100);
            $sum = 0;
            foreach ($osFreqs as $freq => $choice) {
                $sum += $freq;
                if ($rand <= $sum) {
                    return $choice;
                }
            }
        }
    }
    throw new Exception("Frequencies don't sum to 100.");
}
    
function price_check_array_random(array $array) {
    return $array[array_rand($array, 1)];
}
function price_check_nt_version() {
    return rand(5, 6) . '.' . rand(0, 1);
}
function price_check_ie_version() {
    return rand(7, 9) . '.0';
}
function price_check_trident_version() {
    return rand(3, 5) . '.' . rand(0, 1);
}
function price_check_osx_version() {
    return "10_" . rand(5, 7) . '_' . rand(0, 9);
}
function price_check_chrome_version() {
    return rand(13, 15) . '.0.' . rand(800, 899) . '.0';
}
function price_check_presto_version() {
    return '2.9.' . rand(160, 190);
}
function price_check_presto_version2() {
    return rand(10, 12) . '.00';
}
function price_check_firefox($arch) {
    $ver = array_random(array(
	    'Gecko/' . date('Ymd', rand(strtotime('2011-1-1'), time())) . ' Firefox/' . rand(5, 7) . '.0',
	    'Gecko/' . date('Ymd', rand(strtotime('2011-1-1'), time())) . ' Firefox/' . rand(5, 7) . '.0.1',
	    'Gecko/' . date('Ymd', rand(strtotime('2010-1-1'), time())) . ' Firefox/3.6.' . rand(1, 20),
        'Gecko/' . date('Ymd', rand(strtotime('2010-1-1'), time())) . ' Firefox/3.8'
    ));
    switch ($arch) {
    case 'lin':
        return "(X11; Linux {proc}; rv:" . rand(5, 7) . ".0) $ver";
    case 'mac':
        $osx = price_check_osx_version();
        return "(Macintosh; {proc} Mac OS X $osx rv:" . rand(2, 6) . ".0) $ver";
    case 'win':
    default:
        $nt = price_check_nt_version();
        return "(Windows NT $nt; {lang}; rv:1.9." . rand(0, 2) . ".20) $ver";
    }
}
function price_check_safari($arch) {
    $saf = rand(531, 535) . '.' . rand(1, 50) . '.' . rand(1, 7);
    if (rand(0, 1) == 0) {
        $ver = rand(4, 5) . '.' . rand(0, 1);
    } else {
        $ver = rand(4, 5) . '.0.' . rand(1, 5);
    }
    switch ($arch) {
    case 'mac':
        $osx = price_check_osx_version();
        return "(Macintosh; U; {proc} Mac OS X $osx rv:" . rand(2, 6) . ".0; {lang}) AppleWebKit/$saf (KHTML, like Gecko) Version/$ver Safari/$saf";
    //case 'iphone':
    //    return '(iPod; U; CPU iPhone OS ' . rand(3, 4) . '_' . rand(0, 3) . " like Mac OS X; {lang}) AppleWebKit/$saf (KHTML, like Gecko) Version/" . rand(3, 4) . ".0.5 Mobile/8B" . rand(111, 119) . " Safari/6$saf";
    case 'win':
    default:
        $nt = price_check_nt_version();
        return "(Windows; U; Windows NT $nt) AppleWebKit/$saf (KHTML, like Gecko) Version/$ver Safari/$saf";
    }
}
function price_check_iexplorer($arch) {
    $ie_extra = array(
        '',
        '; .NET CLR 1.1.' . rand(4320, 4325) . '',
        '; WOW64'
    );
    $nt = price_check_nt_version();
    $ie = price_check_ie_version();
    $trident = trident_version();
    return "(compatible; MSIE $ie; Windows NT $nt; Trident/$trident)";
}
function price_check_opera($arch) {
    $op_extra = array(
        '',
        '; .NET CLR 1.1.' . rand(4320, 4325) . '',
        '; WOW64'
    );
    $presto = price_check_presto_version();
    $version = price_check_presto_version2();
    switch ($arch) {
    case 'lin':
        return "(X11; Linux {proc}; U; {lang}) Presto/$presto Version/$version";
    case 'win':
    default:
        $nt = price_check_nt_version();
        return "(Windows NT $nt; U; {lang}) Presto/$presto Version/$version";
    }
}
function price_check_chrome($arch) {
    $saf = rand(531, 536) . rand(0, 2);
    $chrome = price_check_chrome_version();
    switch ($arch) {
    case 'lin':
        return "(X11; Linux {proc}) AppleWebKit/$saf (KHTML, like Gecko) Chrome/$chrome Safari/$saf";
    case 'mac':
        $osx = price_check_osx_version();
        return "(Macintosh; U; {proc} Mac OS X $osx) AppleWebKit/$saf (KHTML, like Gecko) Chrome/$chrome Safari/$saf";
    case 'win':
    default:
        $nt = price_check_nt_version();
        return "(Windows NT $nt) AppleWebKit/$saf (KHTML, like Gecko) Chrome/$chrome Safari/$saf";
    }
}
/**
 * Main function which will choose random browser
 * @param  array $lang  languages to choose from
 * @return string       user agent
 */
function price_check_random_uagent(array $lang=array('en-US')) {
    list($browser, $os) = price_check_chooseRandomBrowserAndOs();
    $proc = array(
        'lin' => array('i686', 'x86_64'),
        'mac' => array('Intel', 'PPC', 'U; Intel', 'U; PPC'),
        'win' => array('foo')
    );
    switch ($browser) {
	case 'firefox':
        $ua = "Mozilla/5.0 " . price_check_firefox($os);
        break;
    case 'safari':
        $ua = "Mozilla/5.0 " . price_check_safari($os);
        break;
    case 'iexplorer':
        $ua = "Mozilla/5.0 " . price_check_iexplorer($os);
        break;
    case 'opera':
        $ua = "Opera/" . rand(8, 9) . '.' . rand(10, 99) . ' ' . price_check_opera($os);
        break;
    case 'chrome':
        $ua = 'Mozilla/5.0 ' . price_check_chrome($os);
        break;
    }
    $ua = str_replace('{proc}', price_check_array_random($proc[$os]), $ua);
    $ua = str_replace('{lang}', price_check_array_random($lang), $ua);
    return $ua;
}

function price_check_strip_tags($html) { 
  $search = array('@<script[^>]*?>.*?</script>@si',  // Strip out javascript 
    '@<[\/\!]*?[^<>]*?>@si',            // Strip out HTML tags 
    '@<style[^>]*?>.*?</style>@siU',    // Strip style tags properly 
    '@<![\s\S]*?--[ \t\n\r]*>@'         // Strip multi-line comments including CDATA 
  ); 
  $text = preg_replace($search, '', $html); 
  return $text;
} 

function price_check_make_table_row($items) {
  $items = price_check_force_json_array($items);
  $output = "<tr>";
  foreach($items as $item) {
    $output .= "<td>$item</td>";
  }
  $output .= "</tr>";
  return $output;
}

function price_check_make_link($link, $text, $prompt = false, $prompt_text = "Are you sure?") {
  $prompt_html = "";
  if($prompt) $prompt_html = "onclick='return confirm(\"$prompt_text\")'";
  $output = "<a href='$link'$prompt_html>$text</a>";  
  return $output;
}

function price_check_force_json_array($item) {
  if(is_array($item) && array_key_exists(0, $item)) return $item;
  return array($item);
}

function price_check_sql_insert_field($field, $table, $ext) {
  return "insert into field_{$table}_field_{$field} (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_{$field}_{$ext}) ";
}

function price_check_sql_select_empty_field($field, $table, $bundle) {
  return "select 'node', '$bundle', 0, nid, nid, 'und', 0, '' from node where nid not in (select entity_id from field_{$table}_field_{$field} where bundle='$bundle') and type='$bundle'";
}

function price_check_get_item_template_from_ebay_id_old($ebay_id) {
  $sql = "select nid from field_data_field_subscribers where field_subscribers_value like '%|$ebay_id%";
  $result = db_query($sql);
  $nid = $result->fetchField();
  if($nid) return node_load($nid);
  return false;
}

function price_check_get_item_template_from_ebay_id($ebay_id) {
  global $user;
  $uid = $user->uid;
  $sql = <<<EOSQL
select nid from node n join field_data_field_subscribers fs on fs.entity_id = n.nid where 
uid = :uid and type = 'item_template' and field_subscribers_value like '%|$ebay_id%'
EOSQL;
  $result = db_query($sql, array('uid' => $uid));
  $nid = $result->fetchField();
  if($nid) return node_load($nid);
  return false;
}

function price_check_get_blacklist($lower = false) {
  $sql = <<<EOSQL
  select field_ebay_accounts_value from users u 
join users_roles ur on u.uid = ur.uid
join role r on r.rid = ur.rid
join field_data_field_ebay_accounts fa on fa.entity_id = u.uid
where r.name = 'Paying Customer'
EOSQL;
  $results = db_query($sql);
  foreach($results as $row) {
    if($lower) {
      $output[] = strtolower($row->field_ebay_accounts_value);
    } else {
      $output[] = $row->field_ebay_accounts_value;
    }
  }
  return $output;
}

function price_check_revise_ebay_item($item_template) {
  $subscribers_raw = price_check_decode_field($item_template->field_subscribers['und']);
  foreach($subscribers_raw as $subscriber_raw) {
    if(!$subscriber_raw) continue;
    $subscriber_data = price_check_get_subscriber_from_string($subscriber_raw);
    $ebay_id = $subscriber_data['ebay_id'];
    $ebay_account = $subscriber_data['ebay_account'];
    $auth_token = price_check_get_ebay_auth_code($ebay_account);
    $result = price_check_ebay_revise_listed_item($item_template, $ebay_id, $auth_token, true);
    if(@$result['ItemID']) {
      drupal_set_message('Item updated on eBay');
    } else {
      print_r($result); exit(0);
      drupal_set_message('Update failed. Contact Support');
    }
  }
}

function price_check_modify_item_ajax() {
  $work = array();
  foreach($_POST as $key => $val) {
    if($val != '') price_check_add_work_item($work, $key, $val);
  }
  price_check_clean_work_item($work);
  $operations = array();
  foreach($work as $nid => $params) {
    $subscriber = price_check_get_subscriber_from_string($params['x']);
    $auth_code = price_check_get_ebay_auth_code($subscriber['ebay_account']);
    $operations[] = array(
      'price_check_modify_item_request',
      array(
        $nid,
        $subscriber,
        $auth_code,
        $params,
      ),
    );
  }
  if($operations) {
    $batch = array(
      'title' => 'Updating your eBay items',
      'operations' => $operations,
      'finished' => 'price_check_modify_item_finished',
      'file' => drupal_get_path('module', 'price_check')  . '/common.inc',
    );
    batch_set($batch);
    batch_process('my_listings');
  } else { 
    header("Location: /my_listings");
  }
}

function price_check_modify_item_request($nid, $subscriber, $auth_code, $params, &$context) {
  $result = price_check_update_ebay_item($subscriber['ebay_id'], $auth_code, $params);
  if(@$result['success']) {
    $item = node_load($nid);
    price_check_update_subscriber_info($item, $subscriber, $params);
    node_save($item);
    $context['results'][] = "Updated <a href='" . price_check_get_ebay_item_url($subscriber['ebay_id'])
        . "'>". $item->title . "</a>" . price_check_translate_modify_params($params);
  } else {
    $context['results'][] = "Failed to update ". $item->title . ": " . $result['error'];
  }
}

function price_check_modify_item_finished($success, $results, $operations)  {
  foreach($results as $result) {
    drupal_set_message($result);
  }
}

function price_check_clean_work_item(&$work) {
  foreach($work as $k=>$val) {
    if(@$val['p'] == '' && @$val['q']== '') unset($work[$k]);
  }
}

function price_check_translate_errors($error_xml) {
  if(@$error_xml['ShortMessage']) return $error_xml['ShortMessage'];
  foreach($error_xml as $error) {
    $output[] = $error['ShortMessage'];
  }
  return implode(' ', $output);
}

function price_check_update_subscriber_info(&$item, $subscriber, $params) {
  if(!@$params['p']) return;
  $subscribers = price_check_decode_field($item->field_subscribers['und']);
  foreach($subscribers as $key => $val) {
    if(strpos($val, "{$subscriber['ebay_account']}|{$subscriber['ebay_id']}") !== false) {
      $item->field_subscribers['und'][$key]['value'] = "{$subscriber['ebay_account']}|{$subscriber['ebay_id']}";
      $item->field_price['und'][0]['value'] = $params['p'];
      return;  
    }
  }
}

function price_check_partition($list, $p) {
  $listlen = count($list);
  $partlen = floor($listlen / $p);
  $partrem = $listlen % $p;
  $partition = array();
  $mark = 0;
  for($px = 0; $px < $p; $px ++) {
    $incr = ($px < $partrem) ? $partlen + 1 : $partlen;
    $partition[$px] = array_slice($list, $mark, $incr);
    $mark += $incr;
  }
  return $partition;
}

function price_check_write_cron_log($seq, $file, $data) {
  file_put_contents("/var/www/html/sites/default/logs/$seq/$file", $data, FILE_APPEND);
}
    