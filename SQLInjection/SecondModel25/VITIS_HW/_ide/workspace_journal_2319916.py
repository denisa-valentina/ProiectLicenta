# 2026-06-03T15:39:10.574734
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

comp = client.get_component(name="hardware_app")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

platform = client.get_component(name="platform")
status = platform.build()

comp.build()

comp = client.get_component(name="software_app")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.h", "test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software_app")
comp.build()

vitis.dispose()

