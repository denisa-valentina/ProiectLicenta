# 2026-05-30T22:20:50.172162
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="software_program")
comp.build()

client.delete_component(name="platform")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Vivado/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="software_program")

comp = client.create_app_component(name="software_app",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

status = platform.build()

comp = client.get_component(name="software_app")
comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Sources", files=["helloworld.c", "output.h", "result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="xuartlite_low_level_example")

comp = client.create_app_component(name="hardware_app",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

comp = client.get_component(name="hardware_app")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Sources", files=["result_data.h", "test_data.h", "xuartlite_low_level_example.c"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

vitis.dispose()

