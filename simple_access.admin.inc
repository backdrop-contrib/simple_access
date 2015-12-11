<?php

/**
 * @file
 * House all the admin functions in inc to make the foot print a lot
 * smaller.
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
    foreach ($groups as $group) {
      $gid = $group['gid'];
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
        '#markup' => l(t('edit'), 'admin/config/content/simple_access/groups/' . $gid . '/edit') . '&nbsp;' . l(t('delete'), 'admin/config/content/simple_access/groups/' . $gid . '/delete'),
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
    backdrop_goto('admin/config/content/simple_access/groups/add');
  }
}

/**
 * Overview form submit callback.
 */
function simple_access_page_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['groups'] as $gid => $group) {
    db_update('simple_access_groups')
      ->condition('gid', $gid)
      ->fields(array(
        'weight' => $group['weight'],
      ))
      ->execute();
  }
}

/**
 * Simple Access group form.
 */
function simple_access_group_form($form, &$form_state, $group = array()) {
  if (!empty($group)) {
    backdrop_set_title(t('Edit Access Group'));
    $form['gid'] = array(
      '#type' => 'value',
      '#value' => $group['gid'],
    );
  }
  else {
    backdrop_set_title(t('Create Access Group'));
  }
  $group += array('name' => '', 'roles' => array(), 'weight' => 0);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Group name'),
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
  $form_state['redirect'] = 'admin/config/content/simple_access/groups';
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
    'admin/config/content/simple_access/groups',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Delete group confirm submit callback.
 */
function simple_access_delete_group_confirm_submit($form, &$form_state) {
  simple_access_delete_group($form_state['values']['gid']);
  $form_state['redirect'] = 'admin/config/content/simple_access/groups';
}

/**
 * Access Profiles settings form.
 */
function simple_access_profile_list() {
  $form = array();
  $result = db_select('simple_access_profiles', 'p')
    ->fields('p', array('pid', 'name', 'weight'))
    ->orderBy('weight', 'ASC')
    ->orderBy('name', 'ASC')
    ->execute();

  $profiles = $result->fetchAllAssoc('pid', PDO::FETCH_ASSOC);
  if (empty($profiles)) {
    backdrop_set_message(t('You have not yet defined any access profiles.'));
    backdrop_goto('admin/config/content/simple_access/profiles/add');
  }

  $form['profiles'] = array(
    '#tree' => TRUE,
  );
  foreach ($profiles as $row) {
    $form['profiles'][$row['pid']]['name'] = array(
      '#markup' => $row['name'],
    );
    $form['profiles'][$row['pid']]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $row['weight'],
      '#attributes' => array('class' => array('sa-profile-weight')),
    );
    $form['profiles'][$row['pid']]['operations'] = array(
      '#markup' => l(t('edit'), 'admin/config/content/simple_access/profiles/' . $row['pid'] . '/edit') . ' ' . l(t('delete'), 'admin/config/content/simple_access/profiles/' . $row['pid'] . '/delete'),
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
  foreach ($form_state['values']['profiles'] as $pid => $profile) {
    $profile['pid'] = $pid;
    backdrop_write_record('simple_access_profiles', $profile, array('pid'));
  }
}

/**
 * Simple Access profile form.
 * Used for 'add profile'
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
    '#title' => t('Profile name'),
    '#default_value' => isset($profile['name']) ? $profile['name'] : '',
    '#required' => TRUE,
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
  if (!empty($form_state['values']['pid'])) {
    backdrop_write_record('simple_access_profiles', $form_state['values'], array('pid'));
  }
  else {
    backdrop_write_record('simple_access_profiles', $form_state['values']);
  }

  db_delete('simple_access_profiles_access')
    ->condition('pid', $form_state['values']['pid'])
    ->execute();
  if (isset($form_state['values']['access'])) {
    foreach ($form_state['values']['access'] as $gid => $access) {
      if ($access['sa_view'] || $access['sa_update'] || $access['sa_delete']) {
        $access['pid'] = $form_state['values']['pid'];
        $access['gid'] = $gid;
        backdrop_write_record('simple_access_profiles_access', $access);
      }
    }
  }

  $form_state['redirect'] = 'admin/config/content/simple_access/profiles';
}

/**
 * Simple Access apply profile form.
 */
function simple_access_apply_profile_form() {
  backdrop_set_title(t('Apply a profile to a content type'));
  $nodetypelist = node_type_get_names();

	  $form['nodetype'] = array(
    '#type' => 'radios',
    '#title' => t('Types of content'),
    '#default_value' => '',
    '#options' => $nodetypelist,
    '#description' => t('Choose which type of content for the profile'),
		);

	// get list of profiles

	$profilenames = simple_access_get_profiles_select();


  $form['profileid'] = array(
    '#type' => 'radios',
    '#title' => t('Current profiles'),
    '#default_value' => '',
    '#options' => $profilenames,
    '#description' => t('Choose which profile'),
		);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply this'),
  );
  return $form;
}

/**
 * Apply profile form submit callback.
 */
function simple_access_apply_profile_form_submit($form, &$form_state) {
	$selectednodetype = $form['nodetype']['#value'];
	$pid = $form['profileid']['#value'];
	//add entry to data table simple_access_profiles_nodetype
	//requires node type and pid
  db_merge('simple_access_profiles_nodetype')
	        ->key(array('type' => $selectednodetype))
          ->fields(array(
            'pid' => $pid,
            'type' => $selectednodetype,
          ))
          ->execute();

	//Check whether there are config settings for this type of node
	$nodetype = $selectednodetype;
	$nodeconfig = array();
	$nodeconfig = config_get('simple_access.settings','simple_access_' . $nodetype);
	if(!$nodeconfig){
	  $nodeconfig = config_get('simple_access.settings','simple_access_default');
		config_set('simple_access.settings' , 'simple_access_' . $nodetype, $nodeconfig);
	}

	//now step through each node of type $selectednodetype
	//and set data table simple_access_profile_node
	//to record the profile (pid) for each node
	//
	//need an array listing nodes of selected type
	//from table node, get nid for type

	$nodelist = array();
	$nodelist = db_select('node', 'n')
    ->fields('n', array('nid', 'type','title'))
    ->condition('n.type', $selectednodetype)
    ->addTag('node_access')
		->execute()
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);


	foreach($nodelist as $n){
	  db_merge('simple_access_profiles_node')
		      ->key(array('nid' => $n['nid']))
          ->fields(array(
            'nid' => $n['nid'],
            'pid' => $pid
          ))
          ->execute();
		//now update each node

		$nodeprofile = array();
		$nodeprofile['nid'] = $n['nid'];
		$nodeprofile['pid'] = $pid;
		$nodeprofile['type'] = $selectednodetype;

		simple_access_node_save_apply($nodeprofile);
  }
  $form_state['redirect'] = 'admin/config/content/simple_access/profiles';
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

  // Add a submit button
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for module_settings_form().
 */
function simple_access_settings_page_submit($form, &$form_state) {
  $config = config('simple_access.settings');
  $config->set('simple_access_display', $form_state['values']['simple_access_display']);
  $config->set('simple_access_showgroups', $form_state['values']['simple_access_showgroups']);
  $config->save();
  backdrop_set_message(t('The configuration options have been saved.'));
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
    'admin/config/content/simple_access/profiles',
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Confirm delete submit callback.
 */
function simple_access_profile_delete_confirm_submit($form, &$form_state) {
  simple_access_delete_profile($form_state['values']['pid']);
  $form_state['redirect'] = 'admin/config/content/simple_access/profiles';
}
