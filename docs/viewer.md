

# Viewer

This is an online JavaScript renderer of the Mandelbrot fractal.

<button onclick="javascript:mand_update();">Click here if it isn't loading</button>

<div id="info">
  <span id="info_text"></span>
</div>
<div class="container">
<form id="mand_conf" onsubmit="javascript:mand_update();" action="javascript:mand_update();">
  <p>
    <label for="cX">Center X: </label><input type="number" name="cX" step="0.0000001">
    <label for="cY">Center Y: </label><input type="number" name="cY" step="0.0000001"><br/>
  </p>
  <p>
    <label for="Z">Zoom: </label><input type="number" name="Z" step="0.0000001"><br/>
    <label for="iter">Iterations: </label><input name="iter"><br/>
  </p>
  <p>
    <label for="color_scheme">Color Scheme: </label><select onchange="javascript:mand_update();" name="color_scheme">
      <option value="rand" selected="selected">Random</option>
      <option value="mocha">Mocha</option>
      <option value="red">Red</option>
      <option value="blue">Blue</option>
      <option value="green">Green</option>
    </select><br/>
  </p>
</form>
</div>

<button onclick="javascript:mand_update();">Calculate</button>

<br/>

<span id="info"></span>

<div id="fractal_cont" width="100%">
  <canvas id="fractal" width="100%" height="0">
  </canvas>
</div>
