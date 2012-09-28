<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function dlg($title, $path, $height, $width, $class = NULL, $html = FALSE) {
    if (!empty($class))
        $class = $class . ' ';
    else
        $class = '';

    $size = $width.','.$height;
//    return '<a onclick="' . $js_func . "('" . url($url) . "', $height, $width)\">$anchor_text</a>";
    $attr = array('class' => $class . 'modalframe-setup-child'. (!empty($size) ? ' modalframe-setup-size['. $size .']' : ''));
//    if ($more_attributes) {
//        $attr[] = $more_attributes;
//    }
//    watchdog('attr', print_r($attr,true));
    $options = array('attributes' => $attr);
    $options['html'] = $html;
//    watchdog('attr', print_r($options,true));

    return l($title, $path, $options);
}


function replace_table_header_url($replaceValue, $withValue, $inValue) {
    $pattern = "%({$replaceValue}[^&]*)(&.*order=.*&.*sort=[^\"&]*|&.*sort=.*&.*order=[^\"&]*)[^\"]*%";
    return preg_replace($pattern, $withValue . '$2', $inValue);
}

