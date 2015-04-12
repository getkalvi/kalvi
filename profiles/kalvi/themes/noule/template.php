<?php
/**
  * Implements hook_enable()
  */
function noule_enable() {
  drupal_set_message($message = t('Use the jquery_update module for better results.'), $type = 'status');
}

/*
 * Implements hook_preprocess_html().
 */
function noule_preprocess_html(&$variables) {
  drupal_add_css(drupal_get_path('theme', 'noule') . '/libraries/fontawesome/css/font-awesome.min.css');
  if (path_is_admin(current_path())) {
    drupal_add_css(drupal_get_path('theme', 'noule') . '/libraries/iCheck/skins/flat/red.css');
    // iCheck works only with jquery 1.7+ or Zepto
    if(module_exists('jquery_update')) {
      drupal_add_js(drupal_get_path('theme', 'noule') . '/libraries/iCheck/icheck.min.js');
    }
  }
  if(drupal_get_profile() == 'kalvi') {
    $variables['kalvi_menu'] = _noule_kalvi_menu();
  }
}

function _noule_kalvi_menu() {
  $items = array(
    t('Kalvi') => array('icon' => 'home', 'path' => '<front>'),
    t('Dashboard') => array('icon' => 'dashboard', 'path' => 'admin/dashboard'),
    t('Shortcuts') =>array('icon' => 'rocket', 'path' => 'admin/config/user-interface/shortcut'),
    t('Courses') =>array('icon' => 'courses', 'path' => 'admin/kalvi/courses'),
    t('Kalvi Settings') => array('icon' => 'wrench', 'path' => 'admin/kalvi'),
    t('Site Settings') =>array('icon' => 'gear', 'path' => 'admin/config'),
    t('People') =>array('icon' => 'group', 'path' => 'admin/people'),
    t('App Store') => array('icon' => 'puzzle-piece', 'path' => 'admin/apps'),
    t('Look and Feel') =>array('icon' => 'magic', 'path' => 'admin/appearance'),
  );
  $output = '<ul>';
  foreach(array_keys($items) as $k) {
    $dom = '<i class="fa fa-'. $items[$k]['icon'] . '"></i>';
    $dom .= '<div align="center">'. $k . '</div>';
    $output .= '<li>' . l($dom, $items[$k]['path'], array('html' => TRUE)) . '</li>';
  }
  $output .= '</ul>';
  return $output;
}

/**
 * Overrides theme_admin_block_content().
 *
 * Use unordered list markup in both compact and extended mode.
 */
function noule_admin_block_content($variables) {
  $content = $variables['content'];
  $output = '';
  if (!empty($content)) {
    $output = system_admin_compact_mode() ? '<ul class="admin-list compact">' : '<ul class="admin-list">';
    foreach ($content as $item) {
      if (empty($item['localized_options']['attributes']['class'])) {
        $item['localized_options']['attributes']['class'] = array();
      }
      if (!is_array($item['localized_options']['attributes']['class'])) {
        $item['localized_options']['attributes']['class'] = array($item['localized_options']['attributes']['class']);
      }
      $item['localized_options']['attributes']['class'][] = 'title';

      $item['title'] = "<span class='icon'></span>" . filter_xss_admin($item['title']);
      $item['localized_options']['html'] = TRUE;
      if (!empty($item['localized_options']['attributes']['class'])) {
        $item['localized_options']['attributes']['class'] += _noule_icon_classes($item['href']);
      }
      else {
        $item['localized_options']['attributes']['class'] = _noule_icon_classes($item['href']);
      }
      $output .= '<li class="leaf">';
      $output .= l($item['title'], $item['href'], $item['localized_options']);
      if (isset($item['description']) && !system_admin_compact_mode()) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Preprocessor for theme('admin_block').
 */
function noule_preprocess_admin_block(&$vars) {
  // Add icon and classes to admin block titles.
  if (isset($vars['block']['href'])) {
    $vars['block']['localized_options']['attributes']['class'] =  _noule_icon_classes($vars['block']['href']);
  }
  $vars['block']['localized_options']['html'] = TRUE;
  if (isset($vars['block']['link_title'])) {
    $vars['block']['title'] = l("<span class='icon'></span>" . filter_xss_admin($vars['block']['title']), $vars['block']['href'], $vars['block']['localized_options']);
  }

  if (empty($vars['block']['content'])) {
    $vars['block']['content'] = "<div class='admin-block-description description'>{$vars['block']['description']}</div>";
  }
}

/**
 * Override of theme('breadcrumb').
 */
function noule_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (count($breadcrumb) > 0) {
    $lastitem = sizeof($breadcrumb);
    $crumbs = '<div class="breadcrumb flat">';
    $a = 1;
    foreach($breadcrumb as $value) {
      if ($a!=$lastitem){
        $crumbs .= $value;
        $a++;
      }
      else {
        if($value != strip_tags($value)) {
          $crumbs .= $value;
        }
        else {
          $crumbs .= l($value, NULL, array('external' => TRUE, 'fragment' => FALSE));
        }
      }
    }

    $crumbs .= '</div>';
    return $crumbs;
  }
  else {
    return t("Home");
  }
}

/**
 * Override of theme('textfield').
 */
function noule_textfield($variables) {
  $original = theme_textfield($variables);
  return '<div class="control">' . $original . '</div>';
}

/**
 * Override of theme('button').
 */
function noule_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<button' . drupal_attributes($element['#attributes']) . '>' . $element['#value'] . '</button>';
}

/**
 * Override of theme('menu_local_task').
 */
function noule_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<div class="tabs--primary"><ul>';
    $variables['primary']['#suffix'] = '</ul></div>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<div class="tabs--secondary"><ul>';
    $variables['secondary']['#suffix'] = '</ul></div>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;}


/**
 * Override of theme('menu_local_action').
 */
function noule_menu_local_action($variables) {
  $link = $variables['element']['#link'];

  $output = '<li><i class="fa fa-star-o"></i>';
  if (isset($link['href'])) {
    $output .= l($link['title'], $link['href'], isset($link['localized_options']) ? $link['localized_options'] : array());
  }
  elseif (!empty($link['localized_options']['html'])) {
    $output .= $link['title'];
  }
  else {
    $output .= check_plain($link['title']);
  }
  $output .= "</li>\n";

  return $output;
}

/**
 * Override of theme('node_add_list').
 */
function noule_node_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<ul class="node-type-list">';
    foreach ($content as $item) {
      $item['localized_options']['attributes']['class'] = array('title');
      $output .= '<li><i class="fa fa-plus"></i>' . l($item['title'], $item['href'], $item['localized_options']);
      if($item['description']) {
        $output .= '<div class="description">' . filter_xss_admin($item['description']) . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output = '<p>' . t('You have not created any content types yet. Go to the <a href="@create-content">content type creation page</a> to add a new content type.', array('@create-content' => url('admin/structure/types/add'))) . '</p>';
  }
  return $output;
}


/**
 * Generate an icon class from a path. Adopted from Rubik theme
 */
function _noule_icon_classes($path) {
  $classes = array();
  $args = explode('/', $path);
  if ($args[0] === 'admin' || (count($args) > 1 && $args[0] === 'node' && $args[1] === 'add')) {
    // Add a class specifically for the current path that allows non-cascading
    // style targeting.
    $classes[] = 'path-'. str_replace('/', '-', implode('/', $args)) . '-';
    while (count($args)) {
      $classes[] = drupal_html_class('path-'. str_replace('/', '-', implode('/', $args)));
      array_pop($args);
    }
    return $classes;
  }
  return array();
}
