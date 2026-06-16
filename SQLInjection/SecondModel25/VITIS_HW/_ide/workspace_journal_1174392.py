# 2026-05-29T19:28:37.331194
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="software_program")
comp.build()

status = platform.build()

comp = client.get_component(name="xuartlite_low_level_example")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="software_program")
comp.build()

vitis.dispose()

