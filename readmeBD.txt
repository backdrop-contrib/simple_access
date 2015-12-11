---------------------------------------------------
Additional notes about this Backdrop version
by Graham Oliver
---------------------------------------------------

Simple Access uses the Grants method of controlling node access. 
For information about how this works see the notes in node.module
in core, also node_access_example.module in the contributed
module node_access_example within examples.

When porting this module from Drupal 7 to Backdrop I found that
my need was not being met fully. I needed to be able to restrict
access to a specific type of content for a batch of existing nodes
rather than just an an individual basis for each node. 

To do this I have extended the concept within Simple Access of 'profiles',
initially on the basis that one type of node may have a single profile
in Simple Access, though this may comprise more than one group.

There is now an 'apply profile' function which enables an admin user
to choose a type of node, then a profile, and then apply this profile
to all nodes of this type. The realm 'simple_access_profile' is used for this.

At the time of writing there is more work to be done to improve
the user interface.

11 December 2015
