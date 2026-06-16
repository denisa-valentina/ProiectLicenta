# 2026-06-02T20:50:49.540402
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

status = platform.build()

comp = client.get_component(name="software_app")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

status = platform.build()

comp = client.get_component(name="software_app")
comp.build()

status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep", files=["test_data.h"], dest_dir_in_cmp = "src")

status = platform.build()

comp.build()

vitis.dispose()

