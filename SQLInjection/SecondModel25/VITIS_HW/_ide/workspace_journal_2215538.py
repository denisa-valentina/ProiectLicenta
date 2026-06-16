# 2026-05-30T15:29:57.183398
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

comp = client.get_component(name="xuartlite_low_level_example")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.dat"], dest_dir_in_cmp = "src")

platform = client.get_component(name="platform")
status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["result_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software_program")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

