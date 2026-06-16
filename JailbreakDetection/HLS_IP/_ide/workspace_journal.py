# 2026-06-08T21:36:39.543411
import vitis

client = vitis.create_client()
client.set_workspace(path="Vitis_HLS")

comp = client.get_component(name="hls_component")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

vitis.dispose()

