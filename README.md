# Simple Access

The Simple Access module provides an easy way to make nodes accessible
by only a group (or groups) of users. It defaults to only managing
access of view-ability, so that administrators can simply make some
nodes "private". However, the module can also manage access to editing and
deleting of nodes as well, allowing for nodes that are editable
by anyone in a certain role.

## Installation

- Install this module using the official Backdrop CMS instructions at
  <https://backdropcms.org/guide/modules>.

## Configuration

- Configuration settings are at admin/config/content/simple_access.
  From there you define the name and the roles of one or more
  'access groups'.

- Each content type has a new setting 'Access' which, if used
  will restrict access to that content type.

### Creating an access group

Access groups are made up of user roles. Creating an access group
allows you to define the content accessible to those uesrs. By
default, simple_access makes all content public.

1. Once installed, navigate to `administer > access control > simple access > add group`
   and fill in the form fields as required.

### Creating a profile

Profiles are a method of defining the complete security configuration
for a node. So within a single profile the system administrator can
define exactly how a node will provide security. This means that users
with less knowledge can secure nodes without fear of exposing nodes to
users which should not have access.

## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.

## Maintainer

Herb v/d Dool <github.com/herbdool>

## Credits

### Maintainer for Drupal

- ported to Backdrop by Graham Oliver <github.com/Graham-72/>
- Drupal 7 maintainer: Gordon Heydon (gordon)

This port to Backdrop would not, of course, be possible without all
the work done by the developer and maintainer of the Drupal module.
