<?php

function price_check_menu() {
  $items = array();
  $items['modify_item_ajax'] = array( 
    'page callback' => 'price_check_modify_item_ajax', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['ebay_info'] = array( 
    'title' => 'Ebay Api Info', 
    'page callback' => 'price_check_ebay_info', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['my_listings'] = array( 
    'title' => 'My Ebay Listings', 
    'description' => 'My Ebay Listings',
    'page callback' => 'price_check_ebay_listings', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['my_bot'] = array( 
    'title' => 'My Bot', 
    'page callback' => 'price_check_my_bot', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.dev.inc',
  );
  $items['remove_item'] = array( 
    'title' => 'Remove Item', 
    'page callback' => 'price_check_remove_item', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['edit_ebay'] = array( 
    'title' => 'Edit Ebay Item', 
    'page callback' => 'price_check_edit_ebay_item', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['fix_images'] = array( 
    'title' => 'Edit Ebay Item', 
    'page callback' => 'price_check_fix_ebay_images', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['clear_research'] = array( 
    'title' => 'Edit Ebay Item', 
    'page callback' => 'price_check_clear_research', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['customers'] = array( 
    'title' => 'Customers', 
    'page callback' => 'price_check_customers', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'pages.inc',
  );
  $items['customers_devel'] = array( 
    'title' => 'Customers', 
    'page callback' => 'price_check_customers_devel', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'pages.dev.inc',
  );
  $items['ebay_lookup'] = array( 
    'title' => 'My Ebay Listings', 
    'description' => 'My Ebay Listings',
    'page callback' => 'price_check_debug_ebay_listings', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['my_listings_report'] = array( 
    'title' => 'My Ebay Listings Report', 
    'description' => 'My Ebay Listings Report',
    'page callback' => 'price_check_ebay_listings_user_report', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['my_listings_report_a'] = array( 
    'title' => 'My Ebay Listings Report (Anomalies)', 
    'description' => 'My Ebay Listings Report',
    'page callback' => 'price_check_ebay_listings_user_report_a', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['my_listings_report_b'] = array( 
    'title' => 'My Ebay Listings Report (dev)', 
    'page callback' => 'price_check_ebay_listings_user_report_b', 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['listings_user_report_complete'] = array( 
    'title' => 'My Ebay Listings Report', 
    'description' => 'My Ebay Listings Report',
    'page callback' => 'price_check_ebay_listings_user_report_complete', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['terapeak_test'] = array( 
    'title' => 'Terapeak Test', 
    'description' => 'Terapeak Test',
    'page callback' => 'price_check_terapeak_test', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['terapeak_test_2'] = array( 
    'title' => 'Terapeak Test', 
    'description' => 'Terapeak Test',
    'page callback' => 'price_check_terapeak_test_2', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['terapeak_test_3'] = array( 
    'title' => 'Terapeak Test', 
    'description' => 'Terapeak Test',
    'page callback' => 'price_check_terapeak_test_3', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['listings_report'] = array( 
    'title' => 'Listings Report', 
    'description' => 'Listings Report',
    'page callback' => 'price_check_ebay_listings_report', 
    'access arguments' => array('dsgenie reporting functions'),
  );
  $items['view_user_listings'] = array( 
    'title' => 'View User Listings', 
    'description' => 'View User Listings',
    'page callback' => 'price_check_ebay_user_listings_report', 
    'access arguments' => array('dsgenie reporting functions'),
  );
  $items['lookup'] = array( 
    'title' => 'Lookup Walmart Item', 
    'description' => 'Walmart Item Lookup.',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_lookup_form'), 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['lookup-new'] = array( 
    'title' => 'Lookup Walmart Item', 
    'description' => 'Walmart Item Lookup.',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_lookup_form_new'), 
    'access arguments' => array('dsgenie lookup functions'),
  );
  $items['lookup-debug'] = array( 
    'title' => 'Lookup Item', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_lookup_form_debug'), 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['debug_batch'] = array( 
    'title' => 'Debug Batch', 
    'description' => 'Test batch debugging functions.',
    'page callback' => 'price_check_debug_batch', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['debug_batch_revise'] = array( 
    'title' => 'Debug Batch', 
    'description' => 'Test batch debugging functions.',
    'page callback' => 'price_check_debug_batch_revise', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['kmart-lookup'] = array( 
    'title' => 'Lookup Kmart Item', 
    'description' => 'Kmart Item Lookup.',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_kmart_lookup_form'), 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['dashboard'] = array( 
    'title' => 'Dashboard', 
    'description' => 'User dashboard.',
    'page callback' => 'price_check_user_dashboard', 
    'access callback' => TRUE,
  );
  $items['profit'] = array( 
    'title' => 'Order Profits', 
    'description' => 'Ebay Profits',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_profit_form'), 
    'access arguments' => array('dsgenie financial functions'),
  );
  $items['mass_debug'] = array( 
    'title' => 'Order Profits', 
    'description' => 'Ebay Profits',
    'page callback' => 'price_check_mass_update_orders', 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['gift_card_query'] = array( 
    'title' => 'Get Gift Cards', 
    'description' => 'Load a certain value of GCs',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_gift_card_query'), 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['tracking_update'] = array( 
    'title' => 'Update Tracking Info', 
    'description' => 'Update tracking from data dump',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_tracking_update_form'), 
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['get_orders'] = array( 
    'title' => 'Get Orders', 
    'description' => 'Get New Orders',
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_get_orders_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['qFuNkbXfW8'] = array( 
    'page callback' => 'price_check_paypal_ipn',  
    'type' => MENU_CALLBACK,
    'access callback' => TRUE,
  );
  $items['stripe_hook'] = array( 
    'page callback' => 'price_check_stripe_hook',  
    'type' => MENU_CALLBACK,
    'access callback' => TRUE,
  );
  $items['payment_processor'] = array( 
    'page callback' => 'price_check_payment_processor',  
    'type' => MENU_CALLBACK,
    'access callback' => TRUE,
  );
  $items['ajax_get_shipper_from_code'] = array( 
    'title' => 'ajax_get_shipper_from_code', 
    'description' => 'ajax_get_shipper_from_code',
    'page callback' => 'price_check_ajax_get_shipper_from_code',  
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['tracking_upload_ajax'] = array( 
    'title' => 'Catch Post Data',
    'access arguments' => array('access content'),
    'page callback' => 'price_check_tracking_process_ajax_request',
    'type' => MENU_CALLBACK,
  ); 
  $items['ebay_auth'] = array( 
    'title' => 'Authorize User', 
    'description' => 'Authorize User',
    'page callback' => 'price_check_proc_authorize',  
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['list_item'] = array( 
    'title' => 'List Item', 
    'description' => 'List item in ebay account',
    'page callback' => 'price_check_list_ebay_item',  
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['notifications'] = array( 
    'title' => 'Notifications', 
    'page callback' => 'price_check_display_notifications',  
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['replenish'] = array( 
    'title' => 'Replenish Items', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_replenish_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['menu_rebuild'] = array( 
    'title' => 'Rebuild Menus', 
    'page callback' => 'price_check_menu_rebuild',  
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'testing.inc',
  );
  $items['swap_item'] = array( 
    'title' => 'List Item', 
    'page callback' => 'price_check_swap_ebay_item',  
    'access arguments' => array('dsgenie basic functions'),
    'file' => 'pages.inc',
  );
  $items['dismiss-notification'] = array( 
    'page callback' => 'price_check_dismiss_notification',  
    'type' => MENU_CALLBACK,
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['unlink_items'] = array( 
    'page callback' => 'price_check_unlink_items',  
    'type' => MENU_CALLBACK,
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['fix_items'] = array( 
    'title' => 'Fix Items', 
    'description' => 'Fix Items',
    'page callback' => 'price_check_fix_items',  
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['item_search_results'] = array( 
    'title' => 'Search Results Items', 
    'description' => 'Search Results Items',
    'page callback' => 'price_check_item_search_results',  
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['fix_tax_table'] = array( 
    'page callback' => 'price_check_user_set_tax_table',  
    'access arguments' => array('dsgenie basic functions'),
  );
  $items['user/%user/my_signups'] = array( 
    'title' => 'My Signups', 
    'page callback' => 'price_check_my_signups',  
    'access arguments' => array('dsgenie basic functions'),
    'page arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
  );
  $items['my_signups'] = array( 
    'title' => 'My Signups', 
    'page callback' => 'price_check_my_signups',  
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
  );
  $items['view_invoice'] = array( 
    'title' => 'Invoice', 
    'page callback' => 'price_check_view_invoice',  
    'access callback' => TRUE,
    'page arguments' => array(1),
    'type' => MENU_CALLBACK,
  );
  $items['jnPTRneL'] = array( 
    'page callback' => 'price_check_bcm_stripe',  
    'type' => MENU_CALLBACK,
    'access callback' => TRUE,
  );
  $items['get_seller_items'] = array( 
    'title' => 'Get Seller Items', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_get_seller_items_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['get_seller_items_multiple'] = array( 
    'title' => 'Get Seller Items Multiple', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_get_seller_items_multiple_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['get_seller_items_multiple_a'] = array( 
    'title' => 'Get Seller Items Multiple', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_get_seller_items_multiple_form_a'), 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['worker_report'] = array( 
    'title' => 'Worker Report', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_worker_report_form'), 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['fix_walmart_items'] = array( 
    'title' => 'Fix Walmart Items', 
    'page callback' => 'price_check_fix_walmart_items', 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['seller_work'] = array( 
    'title' => 'Work By Seller', 
    'page callback' => 'price_check_seller_work', 
    'access arguments' => array('dsgenie research functions'),
    'page arguments' => array(1),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['my_research'] = array( 
    'title' => 'My Research', 
    'page callback' => 'price_check_my_research', 
    'access arguments' => array('dsgenie research functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['delete_seller'] = array( 
    'title' => 'Work By Seller', 
    'page callback' => 'price_check_delete_seller', 
    'access arguments' => array('dsgenie research functions'),
    'page arguments' => array(1),
    'type' => MENU_CALLBACK,
  );
  $items['give_listings'] = array( 
    'title' => 'Give Listings', 
    'page callback' => 'price_check_give_listings', 
    'access arguments' => array('dsgenie manager functions'),
    'type' => MENU_CALLBACK,
  );
  $items['marketplace'] = array( 
    'title' => 'Gift Card MarketPlace', 
    'page callback' => 'price_check_card_marketplace', 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
  );
  $items['fix_affiliate_links'] = array( 
    'title' => 'Fix Affiliate Links', 
    'page callback' => 'price_check_fix_affiliate_links', 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['search_title'] = array( 
    'title' => 'Search By Title', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_search_title_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['my_sellers'] = array( 
    'title' => 'My Seller List', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_my_sellers_form'), 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,
    'file' => 'pages.inc',
  );
  $items['walmart_upc'] = array( 
    'title' => 'Walmart UPC', 
    'page callback' => 'price_check_walmart_upc', 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,    
    'delivery callback' => 'price_check_walmart_upc_ajax',
  );
  $items['start_spider'] = array( 
    'title' => 'Spider Debug', 
    'page callback' => 'price_check_start_spider', 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['spider_debug'] = array( 
    'title' => 'Spider Debug', 
    'page callback' => 'price_check_spider_debug', 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['transfer_work'] = array( 
    'title' => 'Transfer Work', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_transfer_work_form'),
    'access arguments' => array('dsgenie manager functions'),
    'type' => MENU_CALLBACK,
  );
  $items['lookup-amazon'] = array( 
    'title' => 'Lookup Amazon Item', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_lookup_amazon_form'), 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'pages.inc',
  );
  $items['spider_admin'] = array( 
    'title' => 'Spider Admin', 
    'page callback' => 'price_check_spider_admin', 
    'access arguments' => array('dsgenie reporting functions'),
    'type' => MENU_CALLBACK,
  );
  $items['item_hook'] = array( 
    'title' => 'Item Redirect', 
    'page callback' => 'price_check_item_hook', 
    'access arguments' => array('dsgenie research functions'),
  );
  $items['ebay_api_info'] = array( 
    'title' => 'Edit Ebay Api Details', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_ebay_api_info'),
    'access arguments' => array('dsgenie research functions'),
    'file' => 'pages.inc',
  );
  $items['listing_report'] = array( 
    'title' => 'Listings Report', 
    'page callback' => 'price_check_listing_report', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'pages.inc',
  );
  $items['work_report'] = array( 
    'title' => 'Work Report', 
    'page callback' => 'price_check_work_report', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'pages.inc',
  );
  $items['review_listings'] = array( 
    'title' => 'Review Listings', 
    'page callback' => 'price_check_review_listings', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'pages.inc',
  );
  $items['fix_walmart_invalid'] = array( 
    'title' => 'fix_walmart_invalid', 
    'page callback' => 'price_check_fix_walmart_invalid', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'testing.inc',
  );
  $items['fix_prices_display'] = array( 
    'title' => 'fix_prices_display', 
    'page callback' => 'price_check_fix_prices_display', 
    'access arguments' => array('dsgenie manager functions'),
    'file' => 'testing.inc',
  );
  $items['relink_ebay'] = array( 
    'title' => 'Relink eBay items', 
    'page callback' => 'price_check_relink_ebay_items', 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'testing.inc',
  );
  $items['relink_ebay_output'] = array( 
    'title' => 'Relink eBay items', 
    'page callback' => 'price_check_relink_ebay_items_output', 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'testing.inc',
  );
  $items['cancelled_2_suggestion'] = array( 
    'title' => 'Relink eBay items', 
    'page callback' => 'price_check_cancelled_2_suggestion', 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'pages.dev.inc',
  );
  $items['resize_images'] = array( 
    'title' => 'Resize Images', 
    'page callback' => 'price_check_resize_images', 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'pages.inc',
  );
  $items['universal_lookup'] = array( 
    'title' => 'Universal Lookup', 
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('price_check_universal_lookup_form'), 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'pages.inc',
  );
  $items['convert_skugrid'] = array( 
    'title' => 'Convert SKUGrid', 
    'page callback' => 'price_check_convert_skugrid', 
    'access arguments' => array('dsgenie basic functions'),
    'type' => MENU_CALLBACK,    
    'delivery callback' => 'price_check_convert_skugrid_ajax',
    'file' => 'pages.inc',
  );
  /*$items['skugrid_conversion'] = array( 
    'title' => 'Convert Items to SKUGrid Tracking', 
    'page callback' => 'price_check_skugrid_conversion', 
    'access arguments' => array('dsgenie research functions'),
    'file' => 'skugrid.inc',
  );*/
  return $items;
  
}
