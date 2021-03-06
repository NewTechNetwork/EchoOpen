<?php
// $Id: uploadfield_formatter.inc,v 1.1.2.14 2010/02/14 10:42:49 heshanmw Exp $

/**
 * @file
 * uploadfield formatter hooks and callbacks.
 */

//include common player file

/**
 * Define some constants
 */
define('VIDEO_RENDERING_PENDING', 1);
define('VIDEO_RENDERING_ACTIVE', 5);
define('VIDEO_RENDERING_COMPLETE', 10);
define('VIDEO_RENDERING_FAILED', 20);
/**
 *
 * @param <type> $element
 * @return <type>
 */
function theme_uploadfield_formatter_video_plain($element) {
//  print_r($field = content_fields($element['#field_name'], $element['#type_name']););
// Inside a view $element may contain null data. In that case, just return.
  module_load_include('inc', 'video', '/includes/common');

  if (empty($element['#item']['fid'])) {
    return '';
  }
//print_r($element['#item']['fid']);
  $result = db_query('SELECT vr.status FROM {video_rendering} vr INNER JOIN {files}
      f ON vr.fid = f.fid WHERE vr.fid = f.fid AND f.status = %d AND f.fid = %d',
      FILE_STATUS_PERMANENT, $element['#item']['fid']);
  $status = db_fetch_object($result);
//  print_r($status);
  if($status->status == VIDEO_RENDERING_ACTIVE || $status->status == VIDEO_RENDERING_PENDING) {
    return theme('video_inprogress');
  }
  else if ($status->status == VIDEO_RENDERING_FAILED) {
    return theme('video_encoding_failed');
  }
  else {
    return _video_common_get_player($element);
  }
//  $field = content_fields($element['#field_name']);
//  $item = $element['#item'];
//
//  $item['data']['alt'] = isset($item['data']['alt']) ? $item['data']['alt'] : '';
//  $item['data']['title'] = isset($item['data']['title']) ? $item['data']['title'] : NULL;
//
//  $class = 'uploadfield uploadfield-'. $field['field_name'];
//  return  theme('uploadfield_image', $item, $item['data']['alt'], $item['data']['title'], array('class' => $class));
}


/**
 *
 * @param <type> $element
 * @return <type>
 */

function theme_uploadfield_formatter_video_nodelink($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }
  //  print_r($field);
  //  exit;
  $node = $element['#node'];
  //  $imagetag = theme('uploadfield_formatter_video_plain', $element);
  $use_default_img = $element['#item']['data']['use_default_video_thumb'];
  if($use_default_img) {
    $imagetag = theme('uploadfield_widget_video_thumb', $field['widget']['default_video_thumb']);
  }
  else if($element['#item']['data']['video_thumb']) {
    //    print_r($field['widget']['default_video_thumb']);
      $thumb = array('filepath' => $element['#item']['data']['video_thumb']);
      $imagetag = theme('uploadfield_widget_video_thumb', $thumb);
    }
  $class = 'popups uploadfield uploadfield-nodelink uploadfield-'. $element['#field_name'];
  return l($imagetag, 'node/'. $node->nid, array('attributes' => array('class' => $class), 'html' => TRUE));

}

/**
 * use image cache profiles for video thumb display
 * @param object $element
 * @return HTML image file
 */

function theme_uploadfield_formatter_videolinked($element) {
  // Inside a view $element may contain NULL data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  // Extract the preset name from the formatter name.
  $presetname = substr($element['#formatter'], 0, strrpos($element['#formatter'], '_'));
  $style = 'linked';

  $item = $element['#item'];
  $item['data']['alt'] = isset($item['data']['alt']) ? $item['data']['alt'] : '';
  $item['data']['title'] = isset($item['data']['title']) ? $item['data']['title'] : NULL;

  $node = $element['#node'];
  //  $imagetag = theme('uploadfield_formatter_video_plain', $element);
  $use_default_img = $element['#item']['data']['use_default_video_thumb'];
  if($use_default_img) {
    $thumb_path = $field['widget']['default_video_thumb'];
//    $imagetag = theme('uploadfield_widget_video_thumb', $field['widget']['default_video_thumb']);
  }
  else if($element['#item']['data']['video_thumb']) {
    //    print_r($field['widget']['default_video_thumb']);
//      $thumb = array('filepath' => $element['#item']['data']['video_thumb']);
      $thumb_path = $element['#item']['data']['video_thumb'];
//      $imagetag = theme('uploadfield_widget_video_thumb', $thumb);
  }
//  die($presetname);
  $imagetag = theme('imagecache', $presetname, $thumb_path, $node->title, $node->title);
  $path = empty($item['nid']) ? '' : 'node/'. $item['nid'];
  $class = "imagecache imagecache-$presetname imagecache-$style imagecache-{$element['#formatter']}";
  return l($imagetag, $path, array('attributes' => array('class' => $class), 'html' => TRUE));
}

/**
 *
 * @param <type> $element
 * @return <type>
 */
function theme_uploadfield_formatter_video_imagelink($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  // Inside a view $element may contain null data. In that case, just return.
  if (empty($element['#item']['fid'])) {
    return '';
  }

  $node = $element['#node'];
  //  $imagetag = theme('uploadfield_formatter_video_plain', $element);
  $imagetag = theme('uploadfield_widget_video_thumb', $field['widget']['default_video_thumb']);
  $class = 'popups uploadfield uploadfield-nodelink uploadfield-'. $element['#field_name'];
  return l($imagetag, 'node/'. $node->nid, array('attributes' => array('class' => $class), 'html' => TRUE));
}

/**
 * Displays a "encoding in progress message"
 */
function theme_video_inprogress() {
  return '<div class="video-ffmpeg-helper-inprogress">'. t('This video is currently being processed. Please wait.') . '</div>';
}


/**
 * Display an "encoding failed" message"
 */
function theme_video_encoding_failed() {
  return '<div class="video-ffmpeg-helper-encoding-failed">'. t('The video conversion process has failed. You might want to submit a simpler video format like <em>mpeg</em> or <em>divx avi</em>.<br />If the problem persists please contact website administrators.') . '</div>';
}


/**
 * Implementation of hook_theme().
 */
function video_ffmpeg_helper_theme() {
  return array(

  );
}