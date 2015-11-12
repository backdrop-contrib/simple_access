# Simple Access

The Simple Access module provides an easy way to make nodes accessible
by only a group (or groups) of users. It defaults to only managing 
access of view-ability, so that administrators can simply make some 
nodes "private". However, the module can also manage edit-ability and 
delete-ability of nodes as well, allowing for nodes that are editable 
by anyone in a certain role.

This is a port from the Drupal module of the same name. 

## Status

This is an initial port of the module ready for testing.


## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
  
## Configuration

- Configuration settings are at admin/config/content/simple_access.
  From there you define the name and the roles of one or more
  'access groups'. 

- Each content type has a new setting 'Access' which, if used
  will restrict access to that content type.


## Help & Documentation

See readme.txt for more information about the background to
this module.


## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Current porting to Backdrop

Graham Oliver (github.com/Graham-72/)

## Credits

### Maintainer for Drupal:

- Gordon Heydon (gordon)

### Acknowledgement

This port to Backdrop would not, of course, be possible without all
the work done by the developer and maintainer of the Drupal module.
