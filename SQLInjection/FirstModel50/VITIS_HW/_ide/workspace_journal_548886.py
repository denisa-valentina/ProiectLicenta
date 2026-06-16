# 2026-06-12T21:24:01.844955
import vitis

client = vitis.create_client()
client.set_workspace(path="VITIS_HW")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="hardware")
comp.build()

status = platform.build()

comp = client.get_component(name="software")
comp.build()

client.delete_component(name="software")

comp = client.create_app_component(name="software",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_psu_cortexa53_0")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../Train_Python", files=["output.h", "result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

