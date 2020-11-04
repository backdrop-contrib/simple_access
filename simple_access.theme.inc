<?php
/**
 * @file
 * Provide themes for simple access administration.
 */

/**
 * Theme simple access form.
 */
function theme_simple_access_form($variables) {
  $form =& $variables['form'];
  $options = array();
  $output = '';
  if (isset($form['#admin'])) {
    $variable = array('view' => 1, 'update' => 1, 'delete' => 1);
  }
  else {
    $variable = config_get('simple_access.settings','simple_access_display');
  }
  $options['header'] = array(t('Access Group'));
  if ((isset($variable['view']) && $variable['view']) || isset($form['#admin'])) {
    $options['header'][] = t('View');
  }
  if ((isset($variable['update']) && $variable['update']) || isset($form['#admin'])) {
    $options['header'][] = t('Update');
  }
  if ((isset($variable['delete']) && $variable['delete']) || isset($form['#admin'])) {
    $options['header'][] = t('Delete');
  }

  foreach (element_children($form) as $gid) {
    if (!isset($form['#access']) || $form['#access']) {
      $row = array(
        array('data' => backdrop_render($form[$gid]['name'])),
      );
      if ((isset($variable['view']) && $variable['view']) || isset($form['#admin'])) {
        $row[] = array(
          'data' => backdrop_render($form[$gid]['sa_view']),
        );
      }
      if ((isset($variable['update']) && $variable['update']) || isset($form['#admin'])) {
        $row[] = array(
          'data' => backdrop_render($form[$gid]['sa_update']),
        );
      }
      if ((isset($variable['delete']) && $variable['delete']) || isset($form['#admin'])) {
        $row[] = array(
          'data' => backdrop_render($form[$gid]['sa_delete']),
        );
      }

      $options['rows'][] = $row;
    }
  }

  if (!empty($options['rows'])) {
    $output .= theme('table', $options);
    return $output;
  }
}

/**
 * Theme simple access overview page as a draggable table.
 */
function theme_simple_access_page_overview_list(&$form) {
  backdrop_add_tabledrag('sa-group-list', 'order', 'sibling', 'sa-group-weight');
  $output = '';
  $options = array(
    'header' => array(t('Group'), t('Roles'), t('Weight'), t('Operations')),
    'rows' => array(),
    'attributes' => array('id' => 'sa-group-list'),
  );
  foreach (element_children($form['form'], TRUE) as $gid) {
    $options['rows'][] = array(
      'data' => array(
        backdrop_render($form['form'][$gid]['name']),
        array('data' => backdrop_render($form['form'][$gid]['roles']), 'class' => 'sa-group-roles'),
        backdrop_render($form['form'][$gid]['weight']),
        backdrop_render($form['form'][$gid]['ops']),
      ),
      'class' => array('draggable'),
    );
  }

  if (empty($options['rows'])) {
    $options['rows'][] = array(
      array(
        'data' => t('No profiles defined'),
        'colspan' => 4,
        'align' => 'center',
      ),
    );
  }
  $output .= theme('table', $options);

  return $output;
}

/**
 * Theme a list of simple access profiles as draggable table.
 */
function theme_simple_access_profile_list($variables) {
  $form =& $variables['form'];

  $rows = array();
  foreach (element_children($form['profiles']) as $id) {
	  $form['profiles'][$id]['weight']['attribute']['class'] = array('sa-profile-weight');
    $row = array(
      'data' => array(
        backdrop_render($form['profiles'][$id]['name']),
        backdrop_render($form['profiles'][$id]['weight']),
        backdrop_render($form['profiles'][$id]['operations']),
      ),
      'class' => array('draggable'),
    );
		$rows[] = $row;
  }
  $header = array(t('Name'), t('Weight'), t('Operations'));
	$output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'sa-profile')));
	$output .= backdrop_render_children($form);

	backdrop_add_tabledrag('sa-profile', 'order', 'sibling', 'sa-profile-weight');

  return $output;
}
