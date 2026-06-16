# 2026-05-30T12:00:04.573416
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_After")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Vivado/project_1/design_1_wrapper.xsa",os = "standalone",cpu = "psu_cortexa53_0",domain_name = "standalone_psu_cortexa53_0")

platform = client.get_component(name="platform")
status = platform.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../FirstStep", files=["test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.create_app_component(name="software",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0",template = "hello_world")

comp = client.get_component(name="software")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../FirstStep", files=["output.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../FirstStep", files=["test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

