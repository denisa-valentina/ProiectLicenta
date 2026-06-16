# 2026-05-28T20:57:22.508436
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Vivado/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/..", files=["test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.create_app_component(name="software_execution",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

comp = client.get_component(name="software_execution")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["output.c"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="software_execution")

comp = client.create_app_component(name="software_program",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

comp = client.get_component(name="software_program")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["test_data.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["output.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

status = platform.build()

comp = client.get_component(name="software_program")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

vitis.dispose()

