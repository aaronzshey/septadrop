use sfml::graphics::IntRect;

pub struct TileType {
    pub texture_rect: IntRect,
    pub ghost_texture_rect: IntRect,
}

impl TileType {
    pub fn new(texture_rect: IntRect, ghost_texture_rect: IntRect) -> Self {
        Self {
            texture_rect,
            ghost_texture_rect,
        }
    }
}
