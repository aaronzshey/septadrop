use crate::config::*;
use crate::structs::TileType;

pub struct Grid<'a> {
    grid: [[Option<&'a TileType>; GRID_WIDTH as usize]; GRID_HEIGHT as usize],
}

impl<'a> Grid<'a> {
    pub fn new() -> Self {
        Self {
            grid: Default::default()
        }
    }

    pub fn out_of_bounds(&self, x: i32, y: i32) -> bool {
        x < 0 || x >= GRID_WIDTH as i32 || y < 0 || y >= GRID_HEIGHT as i32
    }

    pub fn get(&self, x: i32, y: i32) -> Option<&'a TileType> {
        if self.out_of_bounds(x, y) {
            None
        } else {
            self.grid[y as usize][x as usize]   
        }
    }

	// Checks if a coordinate is filled, either by a tile or out of bounds space
	// For checking if there is a tile specifically, do .get(x, y).is_some()
    pub fn filled(&self, x: i32, y: i32) -> bool {
        self.out_of_bounds(x, y) || self.get(x, y).is_some()
    }

    pub fn set(&mut self, x: i32, y: i32, tile_type: Option<&'a TileType>) {
        if !self.out_of_bounds(x, y) {
            self.grid[y as usize][x as usize] = tile_type;
        }
    }

    pub fn clear(&mut self) {
        self.grid = Default::default();
    }
}