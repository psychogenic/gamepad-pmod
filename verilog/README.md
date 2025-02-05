# Gamepad PMOD verilog module

This module provides plug & play support for the gamepad pmod in your projects

Include the [gamepad_pmod.v](gamepad_pmod.v) source file in your build and, somewhere in your project, instantiate one of 

	* gamepad_pmod_dual -- to support one or two controllers, or
	
	* gamepad_pmod_single -- to only support a single controller

You then feed the gamepad module the input data, clock and latch from the outside world and from there you receive information about which buttons are pressed at any time.  The module outputs split up the buttons such that they have reasonable names and they will be HIGH as long as the button in question is held pressed by the user.

## single controller support

If you want to save on bits, you may use the `gamepad_pmod_single`.  This only latches in the last 12 bits of data, so might give you a little bit of space saving in term of flops but you'll be losing data for one player if two controllers are physically connected.

It provides this interface

```
module gamepad_pmod_single (
    input wire rst_n,
    input wire clk,
    input wire pmod_data,
    input wire pmod_clk,
    input wire pmod_latch,

    output wire b,
    output wire y,
    output wire select,
    output wire start,
    output wire up,
    output wire down,
    output wire left,
    output wire right,
    output wire a,
    output wire x,
    output wire l,
    output wire r,
    output wire is_present
);
```

Other than the clock and reset, it takes in three wires from the PMOD for data, clock and latch.  The outputs are split as individual bits, `start`, `left`, `right` etc.  These will remain HIGH so long as the button is held.

In addition to the standard controller buttons, there is an `is_present` field to let you know whether there's a controller connected.


## dual controller support

At the cost of shuffling 12 extra bits around, you can support up to two controllers using `gamepad_pmod_dual`.  This module takes the same inputs as the single version, and basically provides the same API, but instead of single wires for the buttons, each is two bits wide, so you get the status of each controller indexed within (e.g. if  `is_present[0]` and `is_present[1]` are both HIGH, you have two controllers connected).

The module API is thus:

```
module gamepad_pmod_dual (
    input wire rst_n,
    input wire clk,
    input wire pmod_data,
    input wire pmod_clk,
    input wire pmod_latch,

    output wire [1:0] b,
    output wire [1:0] y,
    output wire [1:0] select,
    output wire [1:0] start,
    output wire [1:0] up,
    output wire [1:0] down,
    output wire [1:0] left,
    output wire [1:0] right,
    output wire [1:0] a,
    output wire [1:0] x,
    output wire [1:0] l,
    output wire [1:0] r,
    output wire [1:0] is_present
);
```
 
## Including in a project


Including in a project is just a question of instantiating a module to wire it up and then acting according to button presses reported.  It might look something like:

```
module my_top (
    input  wire [7:0] ui_in,    // Dedicated inputs
    output wire [7:0] uo_out,   // Dedicated outputs
    input  wire [7:0] uio_in,   // IOs: Input path
    output wire [7:0] uio_out,  // IOs: Output path
    output wire [7:0] uio_oe,   // IOs: Enable path (active high: 0=input, 1=output)
    input  wire       ena,      // always 1 when the design is powered, so you can ignore it
    input  wire       clk,      // clock
    input  wire       rst_n     // reset_n - low to reset
);

  // Gamepad Pmod support
  wire gamepad_pmod_latch = ui_in[4];
  wire gamepad_pmod_clk = ui_in[5];
  wire gamepad_pmod_data = ui_in[6];
  wire gamepad_is_present;
  wire gamepad_left;
  wire gamepad_right;
  wire gamepad_up;
  wire gamepad_down;
  wire gamepad_start;
  wire gamepad_select;

  gamepad_pmod_single gamepad_pmod (
      // Inputs:
      .clk(clk),
      .rst_n(rst_n),
      .pmod_latch(gamepad_pmod_latch),
      .pmod_clk(gamepad_pmod_clk),
      .pmod_data(gamepad_pmod_data),

      // Outputs:
      .is_present(gamepad_is_present),
      .left(gamepad_left),
      .right(gamepad_right),
      .up(gamepad_up),
      .down(gamepad_down),
      .start(gamepad_start),
      .select(gamepad_select)
  );
  // do things when buttons are pressed and gamepad_* go high.
  
endmodule

```

A nice example of an implementation using this module is Uri's [2048 game](https://github.com/urish/tt10-2048-game).
