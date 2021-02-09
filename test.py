import sys
import glfw
import moderngl as mgl
from timeit import default_timer
import numpy as np

import polyline2d as p2d

vs = '''
#version 330
in vec2 vertices;
void main() {
    gl_Position = vec4(vertices, 0.0, 1.0);
}
'''

fs = '''
#version 330
uniform vec4 color;
out vec4 o_color;

void main() {
    o_color = color;
}
'''

if not glfw.init():
    sys.exit(1)

glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

win = glfw.create_window(800, 800, 'Test', None, None)
if not win:
    glfw.terminate()
    sys.exit(1)

glfw.make_context_current(win)
ctx = mgl.create_context(require=330)
ctx.viewport = (0, 0, 800, 800)
ctx.enable(mgl.BLEND)
ctx.blend_func = mgl.SRC_ALPHA, mgl.ONE_MINUS_SRC_ALPHA
shader = ctx.program(vertex_shader=vs, fragment_shader=fs)
lightred = 1, 0.2, 0, 0.5
darkred = 1, 0.2, 0, 0.9

pts = np.empty(6, dtype=[('x', 'f4'), ('y', 'f4')])
pts['x'] = [-0.25, -0.25, 0.25, 0.0, 0.25, -0.4]
pts['y'] = [-0.5, 0.5, 0.25, 0.0, -0.25, -0.25]
ret = p2d.create(pts, 0.1, p2d.JointStyle.ROUND, p2d.EndCapStyle.SQUARE)
vbo = ctx.buffer(ret)
vao = ctx.vertex_array(shader, [(vbo, '2f', 'vertices')])

t0 = default_timer()
while default_timer() < (t0 + 5):
    glfw.poll_events()
    ctx.clear(0.1, 0.1, 0.1, 1)
    shader['color'].value = lightred
    vao.render(mgl.TRIANGLES)
    shader['color'].value = darkred
    vao.render(mgl.LINE_STRIP)
    glfw.swap_buffers(win)
