# 2026-06-12T16:17:31.599835
import vitis

client = vitis.create_client()
client.set_workspace(path="BrokenModelHardware")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../RetryModel/project_1/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.create_app_component(name="app_component",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

comp = client.get_component(name="app_component")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Github/RandomForest_v2/RandomForest/FirstStep_copy", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.create_app_component(name="app_component_sw",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

comp = client.get_component(name="app_component_sw")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Github/RandomForest_v2/RandomForest/FirstStep_copy", files=["output.h", "result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp = client.get_component(name="app_component_sw")
comp.build()

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

