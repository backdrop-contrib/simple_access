<?php
/**
 * @file
 * Views access plugin to make use of cimple access.
 */

class simple_access_views_plugin_group extends views_plugin_access {
  /**
   * {@inheritdoc}
   */
  public function access($account) {
    $roles = array_keys($account->roles);
    $roles[] = $account->uid ? DRUPAL_AUTHENTICATED_RID : DRUPAL_ANONYMOUS_RID;

    $groups = simple_access_groups_from_roles($roles);
    return array_intersect(array_filter($this->options['groups']), $groups);
  }

  /**
   * {@inheritdoc}
   */
  public function get_access_callback() {
    return array('simple_access_groups_check_user', array(array_filter($this->options['groups'])));
  }

  /**
   * {@inheritdoc}
   */
  public function summary_title() {
    $count = count($this->options['groups']);
    if ($count < 1) {
      return t('No group(s) selected');
    }
    elseif ($count > 1) {
      return t('Multiple groups');
    }
    else {
      $gids = array_map(array('simple_access_views_plugin_group', '_map_groups'), simple_access_get_groups());
      $gid = array_shift($this->options['groups']);
      return $gids[$gid];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function option_defaults(&$options) {
    $options['groups'] = array();
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = array_map(array('simple_access_views_plugin_group', '_map_groups'), simple_access_get_groups());
    $form['groups'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Simple access groups'),
      '#default_value' => $this->options['groups'],
      '#options' => $options,
      '#description' => t('Only the checked simple access groups will be able to access this display. Note that users with "access all views" can see any view, regardless of role.'),
    );
  }

  /**
   * Callback for array_map.
   */
  public function _map_groups($a) {
    return $a['name'];
  }

  /**
   * Validate the options form.
   */
  public function options_validate(&$form, &$form_state) {
    if (!array_filter($form_state['values']['access_options']['groups'])) {
      form_error($form['role'], t('You must select at least one group if type is "by group"'));
    }
  }

  /**
   * Submit the options form.
   */
  public function options_submit(&$form, &$form_state) {
    // I hate checkboxes.
    $form_state['values']['access_options']['groups'] = array_filter($form_state['values']['access_options']['groups']);
  }
}
