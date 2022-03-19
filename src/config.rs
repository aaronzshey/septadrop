use const_format::formatcp;

pub const TILE_SIZE: u32 = 20;

pub const GRID_WIDTH: u32 = 14;
pub const GRID_HEIGHT: u32 = 20;

pub const WINDOW_WIDTH: u32 = 500;
pub const WINDOW_HEIGHT: u32 = 440;

pub const PLAYFIELD_X: u32 = 20;
pub const PLAYFIELD_Y: u32 = 20;

pub const NEXT_X: u32 = 370;
pub const NEXT_Y: u32 = 70;
pub const NEXT_WIDTH: u32 = 5;
pub const NEXT_HEIGHT: u32 = 5;

pub const LINES_PER_LEVEL: u32 = 5;
pub const POINTS_1_LINE: u32 = 40;
pub const POINTS_2_LINES: u32 = 100;
pub const POINTS_3_LINES: u32 = 300;
pub const POINTS_4_LINES: u32 = 1200;

pub const MOVE_FRAME_INTERVAL: u32 = 125;
pub const MAX_FAST_FORWARD_INTERVAL: u32 = 125;

pub const FPS: u32 = 60;

#[cfg(not(debian))]
pub const RES_PATH: &str = "res";

#[cfg(debian)]
pub const RES_PATH: &str = "/usr/games/septadrop";

pub const RES_AUDIO_PATH: &str = formatcp!("{RES_PATH}/audio");
pub const RES_TEXTURES_PATH: &str = formatcp!("{RES_PATH}/textures");
