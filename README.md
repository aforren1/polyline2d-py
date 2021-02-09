See test.py for an example.

```python
import polyline2d as p2d

pts = [[-0.25, -0.5], [-0.25, 0.5], [0.25, 0.25], [0.0, 0.0], [0.25, -0.25], [-0.4, -0.25]]

ret = p2d.create(pts, 0.1, p2d.JointStyle.ROUND, p2d.EndCapStyle.SQUARE)
```
