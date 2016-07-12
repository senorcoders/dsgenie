<?php

define('DRUPAL_ROOT', dirname(__FILE__));

error_reporting(E_ALL);
ini_set("log_errors", 1);
ini_set("error_log", "./php-error.log");
ini_set('memory_limit', '256M');
require_once './includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    

    
    
    //drupal_flush_all_caches();
    
    
    
    
    //_drupal_flush_css_js();

  registry_rebuild(); print_r("here"); exit(0);
  drupal_clear_css_cache();
  drupal_clear_js_cache();




  // Rebuild the theme data. Note that the module data is rebuilt above, as
  // part of registry_rebuild().
  system_rebuild_theme_data();
  drupal_theme_rebuild();

  entity_info_cache_clear();
  node_types_rebuild();
  // node_menu() defines menu items based on node types so it needs to come
  // after node types are rebuilt.
  menu_rebuild();

  // Synchronize to catch any actions that were added or removed.
  actions_synchronize();

  // Don't clear cache_form - in-progress form submissions may break.
  // Ordered so clearing the page cache will always be the last action.
  $core = array('cache', 'cache_path', 'cache_filter', 'cache_bootstrap', 'cache_page');
  $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
  foreach ($cache_tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }
/*
mail ("kwesi@dsgenie.com" , "db maint1" ,"started at " . time());

exec('mysqldump --user=355840_dsgenie2 --password=WCuq32w7 --host=mysql51-039.wc1.ord1.stabletransit.com 355840_dsgenie2 | ' .
  'mysql --user=355840_dsgenie3 --password=WCuq32w7 --host=mariadb-149.wc1.ord1.stabletransit.com --database=355840_dsgenie3');

mail ("kwesi@dsgenie.com" , "db maint1" ,"finished at " . time());
*/
