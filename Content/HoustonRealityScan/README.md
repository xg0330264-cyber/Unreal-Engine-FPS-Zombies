# Houston RealityScan import

No Houston RealityScan data is included in this repository. Add only scan data you have permission to use:

1. Export the Houston reconstruction from RealityScan in FBX format, including its texture files.
2. In Unreal Editor, import the FBX and textures into `/Game/HoustonRealityScan`.
3. Check the scan's scale and collision, then place the imported mesh over the placeholder city in `AHoustonDriveEnvironment`. Keep the generated road surface/collision until the scan's road surface is verified to be drivable.

The current driving prototype uses a generated block grid and generic primitive car, not a Houston reconstruction.
