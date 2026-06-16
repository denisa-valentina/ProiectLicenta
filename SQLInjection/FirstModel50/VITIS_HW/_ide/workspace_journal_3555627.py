# 2026-06-12T16:53:37.956065
import vitis

client = vitis.create_client()
client.set_workspace(path="BrokenModelHardware")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp = client.get_component(name="app_component_sw")
comp.build()

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp = client.get_component(name="app_component_sw")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp = client.get_component(name="app_component_sw")
comp.build()

vitis.dispose()

