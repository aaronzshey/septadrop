use crate::config::GRID_WIDTH;
use crate::structs::BlockType;
use sfml::system::Vector2i;

pub struct Block<'a> {
    pub block_type: &'a BlockType,
    pub position: Vector2i,
    pub rotation_state: i32,
}

impl<'a> Block<'a> {
    pub fn new(block_type: &'a BlockType) -> Self {
        Self {
            block_type,
            position: Vector2i::new(
                GRID_WIDTH as i32 / 2 - block_type.grid[0].len() as i32 / 2,
                0,
            ),
            rotation_state: 0,
        }
    }

    pub fn get_tiles(&mut self) -> Vec<Vector2i> {
        let mut tiles: Vec<Vector2i> = Vec::new();
        for (y, row) in self.block_type.grid.iter().enumerate() {
            let y = y as i32;
            for (x, cell) in row.iter().enumerate() {
                let x = x as i32;
                if !cell {
                    continue;
                }
                let mut rotated = Vector2i::new(x as i32, y as i32);
                if self.block_type.rotate {
                    let center_x = row.len() as i32 / 2;
                    let center_y = self.block_type.grid.len() as i32 / 2;
                    let offset_x = x - center_x;
                    let offset_y = y - center_y;
                    match self.rotation_state {
                        0 => {}
                        1 => {
                            rotated.x = center_x + offset_y;
                            rotated.y = center_y - offset_x;
                        }
                        2 => {
                            rotated.x = center_x - offset_x;
                            rotated.y = center_y - offset_y;
                        }
                        3 => {
                            rotated.x = center_x - offset_y;
                            rotated.y = center_y + offset_x;
                        }
                        _ => self.rotation_state %= 4,
                    }
                }
                let global = self.position + rotated;
                tiles.push(global);
            }
        }
        tiles
    }
}
