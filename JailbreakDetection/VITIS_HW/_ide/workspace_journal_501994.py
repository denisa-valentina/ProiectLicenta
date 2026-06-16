# 2026-05-30T22:51:10.725030
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_After")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="software")
comp.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

vitis.dispose()

