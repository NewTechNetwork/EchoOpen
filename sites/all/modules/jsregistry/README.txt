Similar in spirit to the theme registry, this module allows core and module javascript files to be overridden by being placed in an alternate location.

The logic is as follows:
1. A module can override a javascript file from drupal's core "misc" folder by placing a file of the same name in the module's "misc" folder. If a module is taking advantage of this, it should list the jsregistry module as one of its dependencies.
2. Files in "sites/<site>/misc" and "sites/all/misc" override files in the module's "misc" and drupal core's "misc" folders.
3. Files in "sites/<site>/misc/module_override/<module_name>" and "sites/all/misc/module_override/<module_name>" override javascript files in a module's folder.

The above is the default logic. This module also invokes hook_js_registry_alter(), so other modules can adjust the registry to something else.
