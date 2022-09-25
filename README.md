# Simple Access

The Simple Access module provides a default View permission to content 
types that can also be managed for each individual node. This makes it
possible to restrict View access, by role or group of roles, to any 
Content Type or to individual nodes. 

This module also provides an option for creating Access Profiles that 
manage the View permission provided by this module along with the 
Update/Edit and Delete permissions that are already available in core.
Access Profiles can be applied to content types or individual nodes.

## Installation

- Install this module using the official Backdrop CMS instructions at
  <https://backdropcms.org/guide/modules>.

## Configuration

- Configuration settings are at admin/config/content/simple_access.
  From there you define the name and the roles of one or more
  'access groups'.

- Each content type has a new setting 'Access' which, if used
  will restrict access to that content type.

### Creating an Access Group

Access Groups simply allow you to group and manage several Roles 
together. Access Groups alone allow you to set the View permission for
each group, but nothing more. By default, simple_access makes all 
content public.

1. Once installed, navigate to `administer > access control > simple access > add group`
   and fill in the form fields as required.

### Creating a profile

Profiles allow administrators to combine the View permission provided by
this module with existing Update and Delete permissions, already available 
in core, for specific Access Groups. By combining these permissions in 
one location it is easier for site admistrators to confidently assign 
permissions without fear of accidentally exposing nodes to site visitors 
that should not have access.

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
