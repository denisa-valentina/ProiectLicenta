# 2026-06-03T16:04:21.729023
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="software_app")
comp.build()

status = platform.build()

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

comp = client.get_component(name="software_app")
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

comp = client.get_component(name="software_app")
comp.build()

status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

status = platform.build()

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

comp.build()

status = platform.build()

comp = client.get_component(name="software_app")
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

comp = client.get_component(name="software_app")
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

vitis.dispose()

