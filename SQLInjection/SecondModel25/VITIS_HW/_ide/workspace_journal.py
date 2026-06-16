# 2026-06-16T21:04:18.829507
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="hardware_app")
comp.build()

vitis.dispose()

