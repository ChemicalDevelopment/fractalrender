
/*

ChemicalDevelopment 2017

*/




var mand_conf_default_dict = [
    ["cX", "-0.5"],
    ["cY", "0.0"],
    ["Z", "0.5"],
    ["iter", "auto"],
    ["color_scheme", "random"],
    ["color_scheme_custom", "255,0,0;0,255,0;0,0,255"]
];

// computed with fractalrender -col XX -colo - --no-image
var colors_str = {
    "red": "21,0,0;42,0,0;63,0,0;85,0,0;106,0,0;127,0,0;148,0,0;170,0,0;191,0,0;212,0,0;233,0,0;255,0,0",
    "green": "0,21,0;0,42,0;0,63,0;0,85,0;0,106,0;0,127,0;0,148,0;0,170,0;0,191,0;0,212,0;0,233,0;0,255,0",
    "blue": "0,0,21;0,0,42;0,0,63;0,0,85;0,0,106;0,0,127;0,0,148;0,0,170;0,0,191;0,0,212;0,0,233;0,0,255",
    "mocha": "0,0,0;21,1,0;42,7,1;63,15,3;85,28,9;106,44,18;127,63,31;148,86,50;170,113,75;191,143,107;212,177,147;233,214,196",
    "random": "RAND",
    "custom": "CUSTOM"
};

var mand_canvas;
var mand_canvas_last;
var mand_canvas_ctx;
var mand_canvas_data;


var num_updates = 0;

var mouse = {
    x: 0,
    y: 0,
    startX: 0,
    startY: 0
};

var mand_canvas_isclicked = false;

var mand_update_count = 0;

var mand_conf_dict = mand_conf_default_dict;

var mand_conf_dict_last;

$(window).on('load', function () {
    mand_init();
});
function setPixel(imageData, x, y, r, g, b, a) {
    index = (x + y * imageData.width) * 4;
    imageData.data[index+0] = r;
    imageData.data[index+1] = g;
    imageData.data[index+2] = b;
    imageData.data[index+3] = a;
}

function unsrl_dict(src, dfts) {
  var ret = {};

  var _keyvals = src.split("&");
  var keyvals = [];

  for (var i = 0; i < _keyvals.length; ++i) {
      keyvals.push(_keyvals[i].split("="));
  }

  for (var i = 0; i < dfts.length; ++i) {
      if (dfts[i][1] != "") {
          ret[dfts[i][0]] = dfts[i][1];
      }
  }

  for (var i = 0; i < keyvals.length; ++i) {
      if (keyvals[i][1] != "") {
          ret[keyvals[i][0]] = keyvals[i][1];
      }
  }

  console.log("src: ", src, ", keyvals: " + keyvals, ", ret: ", ret);

  return ret;
}

function mand_init() {
    mand_conf_dict = mand_conf_default_dict;

    var serialized = window.location.hash.replace("#/viewer?", "");
    serialized = serialized.replace("#/viewer", "");

    console.log(mand_conf_default_dict)

    mand_conf_dict = unsrl_dict(serialized, mand_conf_default_dict);

    setTimeout(function() {
        if (num_updates == 0) {
            if ($('#color_scheme').val() == undefined) {
              $('#color_scheme').val("random");
              //$('#color_scheme_custom').val(mand_conf_default_dict["color_scheme_custom"]);
            }

            mand_update();
        }
    }, 100);

    setTimeout(function () {
      $('#mand_conf').populate(mand_conf_dict);
      $("#color_scheme_custom").val($("#color_scheme_custom").val().replace(new RegExp("%3B", 'g'), ";"));

         $("#mand_conf").keypress(function (e) {
            if (e.keyCode == 13) {
              mand_update();
            }
         });

        $(window).on('resize', function() {

        mand_update();

      });



      mand_canvas = $('#fractal')[0];


      mand_canvas_ctx = mand_canvas.getContext("2d");

      mand_canvas.width = $('#fractal_cont').width();
      mand_canvas.height = mand_canvas.width;

      mand_canvas_ctx.save();


      function setMousePosition(e) {
          var ev = e || window.event; //Moz || IE
          var pdiv = document.getElementById("fractal_cont").getBoundingClientRect();
          var ddiv = document.body.getBoundingClientRect();
          if (ev.pageX) { //Moz
              mouse.x = ev.pageX - window.pageXOffset;
              mouse.y = ev.pageY - window.pageYOffset;
          } else if (ev.clientX) { //IE
              mouse.x = ev.clientX - document.body.scrollLeft;
              mouse.y = ev.clientY - document.body.scrollTop;
          }
          mouse.x = Math.floor(mouse.x - (pdiv.left));
          mouse.y = Math.floor(mouse.y - (pdiv.top));
          //console.log(mouse);
      };


      mand_canvas.onmousemove = function(event) {
          setMousePosition(event);
      }


      mand_canvas.onmousedown = function(event) {

          mand_canvas_isclicked = true;
          var ev = event || window.event; //Moz || IE
          var pdiv = document.getElementById("fractal_cont").getBoundingClientRect();
          var ddiv = document.body.getBoundingClientRect();
          if (ev.pageX) { //Moz
              mouse.startX = ev.pageX - window.pageXOffset;
              mouse.startY = ev.pageY - window.pageYOffset;
          } else if (ev.clientX) { //IE
              mouse.startX = ev.clientX - document.body.scrollLeft;
              mouse.startY = ev.clientY - document.body.scrollTop;
          }

          mouse.startX = Math.floor(mouse.startX - (pdiv.left));
          mouse.startY = Math.floor(mouse.startY - (pdiv.top));
      }

      mand_canvas.onmouseup = function(ev) {
          mand_canvas_isclicked = false;
          setMousePosition(ev);

          console.log(mouse);

          mand_canvas_ctx.putImageData(mand_canvas_last, 0, 0);

          mand_canvas_last = mand_canvas_ctx.getImageData(0, 0, mand_canvas.width, mand_canvas.height);

          mand_canvas_ctx.rect(mouse.startX,mouse.startY,mouse.x-mouse.startX,mouse.y-mouse.startY);

          mand_canvas_ctx.stroke();

          console.log("drew")

          var cZ = parseFloat($('input[name="Z"]').val());

          var c_flt = mand_canvas.width * mand_canvas.height;
          var new_flt = (mouse.y-mouse.startY) * (mouse.x-mouse.startX);

          var new_pix_x = (mouse.x + mouse.startX) / 2;
          var new_pix_y = (mouse.y + mouse.startY) / 2;

          var ccX = parseFloat(mand_conf_dict["cX"]);
          var ccY = parseFloat(mand_conf_dict["cY"]);

          var fact = Math.sqrt(new_flt / c_flt);

          console.log(fact);

          console.log(ev.shiftKey);

          if (ev.shiftKey) {
              fact = 1.0 / fact;
          }

          console.log("Zooming, by factor", fact)
          var new_coord_x = ccX - 1.0 / cZ + 2.0 * new_pix_x / (mand_canvas.width * cZ);
          var new_coord_y = ccY + 1.0 / cZ - 2.0 * new_pix_y / (mand_canvas.width * cZ);

          $('input[name="Z"]').val("" + Math.abs(cZ / fact));
          $('input[name="cX"]').val("" + new_coord_x);
          $('input[name="cY"]').val("" + new_coord_y);

          mand_update();

      }

      mand_update();

    }, 0);


}
function mand_conf_update() {

  $("#color_scheme_custom").val($("#color_scheme_custom").val().replace(new RegExp("\%3B", 'g'), ";"));

    var serialized = $("#mand_conf").serialize();

    mand_conf_dict = unsrl_dict(serialized, mand_conf_default_dict, mand_conf_dict);
    console.log(serialized);
    serialized = "";


    serialized = $("#mand_conf").serialize();

    if (mand_update_count > 0) {
        window.location.hash = "#/viewer?" + serialized;
    }

    mand_update_count++;
}

function color_scheme_from_src(src) {
    var ret = {
        numcol: 0,
        data: []
    }



    if (src == "RAND") {
        ret.numcol = 20;
        for (var i = 0; i < ret.numcol; ++i) {
            ret.data.push([Math.floor(Math.random() * 256), Math.floor(Math.random() * 256), Math.floor(Math.random() * 256)]);
        }

        return ret;
    } else {
        var ssrc;
        if (src == "CUSTOM") {
            ssrc = $("#color_scheme_custom").val().split(";");
        } else {
            ssrc = src.split(";");
        }

        ret.numcol = ssrc.length;

        var scol, tcol;
        for (var i = 0; i < ssrc.length; ++i) {
            scol = ssrc[i].split(",");
            tcol = [];
            if (scol.length != 3) {
                console.log("color source: " + src + " has non-length-3 string");
            }
            for (var j = 0; j < scol.length; ++j) {
                tcol.push(parseInt(scol[j]));
            }
            ret.data.push(tcol);
        }

        console.log(ret.data);

        return ret;
    }
}


function mand_update() {
    console.log("Recalculating...")

    mand_conf_update();


    mand_canvas = $('#fractal')[0];
    mand_canvas_ctx = mand_canvas.getContext("2d");

    mand_canvas.width = $('#fractal_cont').width();
    mand_canvas.height = mand_canvas.width;

    //mand_canvas_ctx = mand_canvas.getContext("2d");
    mand_canvas_data = mand_canvas_ctx.createImageData(mand_canvas.width, mand_canvas.height);

    num_updates++;

    mand_recalc();

    mand_canvas_ctx.putImageData(mand_canvas_data, 0, 0);


    mand_canvas_last = mand_canvas_ctx.getImageData(0, 0, mand_canvas.width, mand_canvas.height);

}


function mand_handle(event) {
    var rect = mand_canvas.getBoundingClientRect();
    var x = event.clientX - rect.left;
    var y = event.clientY - rect.top;
    console.log("x: " + x + " y: " + y);
}

function fmod(a, b) {
    return a - (Math.floor(a / b) * b);
}

function mand_recalc() {
    var cX = parseFloat(mand_conf_dict["cX"]);
    var cY = parseFloat(mand_conf_dict["cY"]);
    var Z = parseFloat(mand_conf_dict["Z"]);
    var iter = mand_conf_dict["iter"];
    var color_scheme_key = mand_conf_dict["color_scheme"];

    if (iter == "auto") {
        iter = Math.round(10 * Math.pow(Z, .5));
    } else {
        iter = parseInt(iter);
    }

    if (iter < 50) {
        iter = 50;
    }

    console.log("MAND_RECALC: cX: " + cX + " cY: " + cY + " Z: " + Z + " iter: " + iter + " color_scheme: " + color_scheme_key);

    var color_scheme = color_scheme_from_src(colors_str[color_scheme_key]);

    var __color_scheme = "";

    for (var i = 0; i < color_scheme.numcol; ++i) {
        __color_scheme = __color_scheme + "" + color_scheme.data[i][0] + "," + color_scheme.data[i][1] + "," + color_scheme.data[i][2];
        if (i != color_scheme.numcol - 1) {
            __color_scheme = __color_scheme + ";";
        }
    }


    var relevant_info = {
        cX: "" + cX,
        cY: "" + cY,
        Z: "" + Z,
        iter: iter,
        color_scheme: "custom",
        color_scheme_custom: __color_scheme
    }

    console.log("Full color scheme: " + JSON.stringify(color_scheme));
    SHARE_URL = "http://chemicaldevelopment.us/fractalrender/#/viewer?" + $.param(relevant_info);
    console.log("URL to share: " + SHARE_URL);
    $("#info_link").text("URL to share")
    $("#info_link").attr("href", SHARE_URL)

    var s_x = cX - 1.0 / Z, s_y;
    var x, y, zn2, scl;
    var x_s, y_s;
    var ci, c0idx, c1idx, idx, tmp;
    var ccol0, ccol1;
    var er = 4.0, er2;
    er2 = er * er;
    var d_c = 2.0 / (Z * mand_canvas_data.width);
    for (i = 0; i < mand_canvas_data.width; i++) {
        s_y = cY + 1.0 / Z;
        for (j = 0; j < mand_canvas_data.height; j++) {
            x = s_x;
            y = s_y;
            x_s = x * x;
            y_s = y * y;
            for (ci = 0; ci < iter && x_s + y_s <= er2; ++ci) {
                tmp = 2 * x * y;
                x = x_s - y_s + s_x;
                y = tmp + s_y;
                x_s = x * x;
                y_s = y * y;
            }

            zn2 = x_s + y_s;

            if (zn2 < er2) {
                scl = 0;
            } else {
                scl = ci + 1.0 - Math.log(Math.abs(zn2)) / Math.log(er2);
            }

            scl = scl * .2;

            scl = fmod(fmod(scl, color_scheme.numcol) + color_scheme.numcol, color_scheme.numcol);

            tmp = scl - Math.floor(scl);

            idx = (i + j * mand_canvas_data.width) * 4;

            c0idx = Math.floor(scl) % color_scheme.numcol;

            if (c0idx >= color_scheme.numcol - 1) {
                c1idx = 0;
            } else {
                c1idx = c0idx + 1;
            }

            ccol0 = color_scheme.data[c0idx];
            ccol1 = color_scheme.data[c1idx];

            mand_canvas_data.data[idx + 0] = parseInt(tmp*ccol1[0]+(1-tmp)*ccol0[0]);
            mand_canvas_data.data[idx + 1] = parseInt(tmp*ccol1[1]+(1-tmp)*ccol0[1]);
            mand_canvas_data.data[idx + 2] = parseInt(tmp*ccol1[2]+(1-tmp)*ccol0[2]);

            //mand_canvas_data.data[idx + 3] = 155 + 100 * ci / iter;
            mand_canvas_data.data[idx + 3] = 255;

            s_y -= d_c;
        }
        s_x += d_c;
    }
}
