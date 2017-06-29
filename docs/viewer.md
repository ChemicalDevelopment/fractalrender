

# Viewer

This is an online JavaScript renderer of the Mandelbrot fractal.

<div id="info">
  <span id="info_text"></span>
</div>
<div class="container">
<form id="mand_conf" onsubmit="javascript:mand_update();" action="javascript:void(0);">
  <p>
    <label for="cX">Center X: </label><input type="number" name="cX" step="0.0000001">
  </p>
  <p>
    <label for="cY">Center Y: </label><input type="number" name="cY" step="0.0000001"><br/>
  </p>
  <input type="submit">
</form>
</div>

<div id="fractal_cont">
  <canvas id="fractal" width="0" height="0">
  </canvas>
</div>
