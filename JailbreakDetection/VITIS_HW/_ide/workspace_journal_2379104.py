# 2026-05-30T17:54:17.991325
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_After")

comp = client.get_component(name="software")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../FirstStep", files=["result_data.h"], dest_dir_in_cmp = "src")

platform = client.get_component(name="platform")
status = platform.build()

comp.build()

status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../FirstStep", files=["output.h"], dest_dir_in_cmp = "src")

status = platform.build()

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

