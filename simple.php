<?php

require_once('simpletest/browser.php');
    
$browser = &new SimpleBrowser();
$browser->useCookies();
$browser->get('wayfair.com/Rockwell-Arm-Chair-CHR-AH-RKWL-LMS2574.html');
$info = strip_tags($browser->getContent());
print $info;
