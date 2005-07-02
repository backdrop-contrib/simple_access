------------------------------------------------------------------------------
  Simple Access Module for Drupal
    developed by: Jeff Robbins | jjeff | robbins (att) jjeff (d0t) com
------------------------------------------------------------------------------

After testing out most of Drupal's node-access modules, I found myself frustrated with their tendency toward confusing user interfaces, defaulting to hiding all nodes, or allowing me to inadvertently make nodes editable or deletable by non-administrators.

Simple Access is an attempt to solve these problems with a focus on simplicity and ease of use. This module provides an easy way to make nodes accessible by only a group (or groups) of users. It defaults to only managing access of viewability, so that administrators can simply make some nodes "private". However, the module can also manage editability and deleteability of nodes as well, allowing for nodes that are editable by anyone in a certain role.

Access groups are based on roles. So for example, my site has roles entitled "Coach Level 1", "Coach Level 2", and "Coach Level 3". I can create an access group called "Coaches" and assign all of these roles to it. Then when I assign a node to be viewable only by "Coaches", only users who are a member of these roles will be able to view.

Nodes that are not assigned to any access groups will remain viewable by all users, so all nodes will remain viewable when activating this module. And nodes only become private when they are assigned to an access group.