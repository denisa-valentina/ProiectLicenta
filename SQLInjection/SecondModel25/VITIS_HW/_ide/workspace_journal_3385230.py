# 2026-06-03T17:46:44.960874
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

client.delete_component(name="hardware_app")

client.delete_component(name="platform")

client.delete_component(name="software_app")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Vivado/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.create_app_component(name="software_app",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

comp = client.get_component(name="software_app")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["output.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

