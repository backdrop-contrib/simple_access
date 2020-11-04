<?php

/**
 * @file
 * Admin functions.
 */

/**
 * Simple Access Overview form.
 */
function simple_access_page_overview() {
  if (count($groups = simple_access_get_groups())) {
    backdrop_set_title(t('Access groups'));
    $roles = user_roles();
    $form['groups'] = array(
      '#tree' => TRUE,
      '#theme' => 'simple_access_page_overview_list',
    );
    foreach ($groups as $gid => $group) {
      $form['groups'][$gid]['name'] = array(
        '#markup' => $group['name'],
      );
      $r = array();
      foreach ($group['roles'] as $role) {
        $r[] = $roles[$role];
      }
      $form['groups'][$gid]['roles'] = array(
        '#markup' => implode(', ', $r),
      );
      $form['groups'][$gid]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $group['weight'],
        '#attributes' => array('class' => array('sa-group-weight')),
      );
      $form['groups'][$gid]['ops'] = array(
        '#markup' => l(t('edit'), 'admin/config/content/simple-access/groups/' . $gid . '/edit') . '&nbsp;' . l(t('delete'), 'admin/config/content/simple-access/groups/' . $gid . '/delete'),
      );
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
    return $form;
  }
  else {
    backdrop_set_message(t('You have not yet defined any access groups.'));
    backdrop_goto('admin/config/content/simple-access/groups/add');
  }
}

/**
 * Overview form submit callback.
 */
function simple_access_page_overview_submit($form, &$form_state) {
  $config = config('simple_access.settings');
  foreach ($form_state['values']['groups'] as $gid => $group) {
    $config->set('groups.' . $gid . '.weight', $group['weight']);
  }
  $config->save();
}

/**
 * Simple Access group form.
 */
function simple_access_group_form($form, &$form_state, $group = array()) {
  $config = config('simple_access.settings');

  if (!empty($group)) {
    backdrop_set_title(t('Edit Access Group'));
    $gid = $group['gid'];
  }
  else {
    backdrop_set_title(t('Create Access Group'));
    $groups = $config->get('groups');
    $gid = 1;
    if (!empty($groups)) {
      $gid = max(array_keys($groups)) + 1;
    }
  }
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $gid,
  );

  $group += array('name' => '', 'roles' => array(), 'weight' => 0);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $group['name'],
    '#size' => 40,
    '#maxlength' => 80,
    '#description' => t('The name for the access group as it will appear on the content editing form.'),
    '#required' => TRUE,
  );
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => $group['roles'],
    '#options' => user_roles(),
    '#description' => t('Roles that can view'),
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => 'Weight',
    '#default_value' => $group['weight'],
    '#delta' => 10,
    '#description' => t('When setting permissions, heavier names will sink and lighter names will be positioned nearer the top.'),
  );
  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Group form submit callback.
 */
function simple_access_group_form_submit($form, &$form_state) {
  simple_access_save_group($form_state['values']);
  $form_state['redirect'] = 'admin/config/content/simple-access/groups';
}

/**
 * Confirm group delete form.
 */
function simple_access_delete_group_confirm($form, $form_state, $group) {
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $group['gid'],
  );
  return confirm_form($form,
    t('Are you sure you want to delete this group?'),
    'admin/config/content/simple-access/groups',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Delete group confirm submit callback.
 */
function simple_access_delete_group_confirm_submit($form, &$form_state) {
  simple_access_delete_group($form_state['values']['gid']);
  $form_state['redirect'] = 'admin/config/content/simple-access/groups';
}

/**
 * Access Profiles settings form.
 */
function simple_access_profile_list() {
  $form = array();
  $profiles = config_get('simple_access.settings', 'profiles');
  if (empty($profiles)) {
    $profiles = array();
  }
  backdrop_sort($profiles);
  if (empty($profiles)) {
    backdrop_set_message(t('You have not yet defined any access profiles.'));
    // backdrop_goto('admin/config/content/simple-access/profiles/add');
  }

  $form['profiles'] = array(
    '#tree' => TRUE,
  );
  foreach ($profiles as $pid => $row) {
    $form['profiles'][$pid]['name'] = array(
      '#markup' => $row['name'],
    );
    $form['profiles'][$pid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $row['weight'],
      '#attributes' => array('class' => array('sa-profile-weight')),
    );
    $form['profiles'][$pid]['operations'] = array(
      '#markup' => l(t('edit'), 'admin/config/content/simple-access/profiles/' . $pid . '/edit') . ' ' . l(t('delete'), 'admin/config/content/simple-access/profiles/' . $pid . '/delete'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Profile list submit callback.
 */
function simple_access_profile_list_submit($form, $form_state) {
  $config = config('simple_access.settings');
  foreach ($form_state['values']['profiles'] as $pid => $profile) {
    $config->set('profiles.' . $pid . '.weight', (int) $profile['weight']);
  }
  $config->save();
}

/**
 * Simple Access profile form.
 */
function simple_access_profile_form($form, $form_state, $profile = array()) {
  $profile += array('access' => FALSE);
  backdrop_set_title(t('Define a profile'));
  if (!empty($profile['pid'])) {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $profile['pid'],
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($profile['name']) ? $profile['name'] : '',
    '#required' => TRUE,
  );

  $form['weight'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($profile['weight']) ? $profile['weight'] : 0,
    '#delta' => 10,
  );

  $form['access'] = array(
    '#tree' => TRUE,
    '#theme' => 'simple_access_form',
  );

  $groups = simple_access_group_select();
  foreach ($groups as $gid => $group) {
    $form['access'][$gid] = simple_access_form_row($gid, $group, $profile['access']);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($profile['pid']) ? t('Submit') : t('Update'),
  );

  return $form;
}

/**
 * Profile form submit callback.
 */
function simple_access_profile_form_submit($form, &$form_state) {
  $config = config('simple_access.settings');
  $profiles = $config->get('profiles');
  $pid = 1;
  if (!empty($profiles)) {
    $pid = max(array_keys($profiles)) + 1;
  }
  if (!empty($form_state['values']['pid'])) {
    $pid = $form_state['values']['pid'];
  }

  $config->set('profiles.' . $pid, array(
    'pid' => (int) $pid,
    'name' => $form_state['values']['name'],
    'weight' => (int) $form_state['values']['weight'],
    'access' => isset($form_state['values']['access']) ? $form_state['values']['access'] : array(),
  ));
  $config->save();

  $form_state['redirect'] = 'admin/config/content/simple-access/profiles';
}

/**
 * Simple Access settings form.
 */
function simple_access_settings_page() {
  backdrop_set_title(t('Simple Access Settings'));
  $options = array(
    'view' => t('<strong>View</strong>: Displays viewability selections at top of node form. Selected access groups will be the only users who can view the node. All unselected = normal node behavior (viewable by all).<br />'),
    'update' => t('<strong>Edit</strong>: Displays editability selections at top of node form. Users who are part of selected access groups will be able to edit this node. All unselected = "normal" node behavior (only author and admins may edit).<br />'),
    'delete' => t('<strong>Delete</strong>: Displays deleteability selections at top of node form. Users who are part of selected access groups will be able to delete this node. All unselected = "normal" node behavior (only author and admins may delete).<br />'),
  );
  $form['#config'] = 'simple_access.settings';
  $form['simple_access_display'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display'),
    '#default_value' => config_get('simple_access.settings','simple_access_display'),
    '#options' => $options,
    '#description' => t('Which options should appear on node add/edit pages for administrators? Select at least one.'),
    '#required' => TRUE,
  );
  $form['simple_access_showgroups'] = array(
    '#type' => 'checkbox',
    '#title' => 'Show groups even when user is not a member.',
    '#default_value' => config_get('simple_access.settings','simple_access_showgroups'),
    '#description' => 'This is useful when you want to have a user be able to make content viewable by themselves and a higher privileged group (e.g. students sharing work with faculty)',
  );
  return system_settings_form($form);
}

/**
 * Confirm delete form.
 */
function simple_access_profile_delete_confirm($form, &$form_state, $profile) {
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $profile['pid'],
  );
  return confirm_form($form,
    t('Are you sure you want to delete this profile?'),
    'admin/config/content/simple-access/profiles',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Confirm delete submit callback.
 */
function simple_access_profile_delete_confirm_submit($form, &$form_state) {
  simple_access_delete_profile($form_state['values']['pid']);
  $form_state['redirect'] = 'admin/config/content/simple-access/profiles';
}
