<?php
//php -c phpltp.ini ./test.php

$aaa=array();
$sss = segmentor_create_segmentor('/var/opt/yxd/ltp/cws.model');
$ccc = segmentor_segment($sss, 'segmentor_create_segmentor', $aaa);
$rrr = segmentor_release_segmentor($sss);
print_r($aaa);




