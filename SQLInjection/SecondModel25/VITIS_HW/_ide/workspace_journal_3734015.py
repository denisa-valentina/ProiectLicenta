# 2026-06-12T16:36:15.271113
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

comp = client.get_component(name="software_app")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../../Github/RandomForest_v2/RandomForest/FirstStep_copy", files=["output.h"], dest_dir_in_cmp = "src")

vitis.dispose()

