use sfml::graphics::*;
use sfml::system::Vector2f;
use sfml::SfBox;

pub struct NumberRenderer {
    texture: SfBox<Texture>,
    comma_rect: IntRect,
    numeral_rects: [IntRect; 10],
}

impl NumberRenderer {
    pub fn new(texture: SfBox<Texture>, comma_rect: IntRect, numeral_rects: [IntRect; 10]) -> Self {
        Self {
            texture,
            comma_rect,
            numeral_rects,
        }
    }

    pub fn default() -> Self {
        Self::new(
            Texture::from_file(&crate::texture("numerals")).unwrap(),
            IntRect::new(134, 0, 10, 16),
            [
                IntRect::new(0, 0, 14, 16),
                IntRect::new(14, 0, 8, 16),
                IntRect::new(22, 0, 14, 16),
                IntRect::new(36, 0, 14, 16),
                IntRect::new(50, 0, 14, 16),
                IntRect::new(64, 0, 14, 16),
                IntRect::new(78, 0, 14, 16),
                IntRect::new(92, 0, 14, 16),
                IntRect::new(106, 0, 14, 16),
                IntRect::new(120, 0, 14, 16),
            ],
        )
    }

    pub fn render(&self, window: &mut RenderWindow, number: u32, x: u32, y: u32) {
        let number_string = number.to_string();
        let get_numeral_rect =
            |numeral: char| self.numeral_rects[numeral.to_digit(10).unwrap() as usize];
        let mut numeral_position = Vector2f::new(
            {
                let numeral = number_string.chars().last().unwrap();
                let numeral_rect = get_numeral_rect(numeral);
                x as f32 - numeral_rect.width as f32
            },
            y as f32,
        );
        let digits = number_string.len();
        let mut sprite = Sprite::new();
        sprite.set_texture(&self.texture, false);
        // can't reverse .chars() directly since it doesn't implement std::iter::DoubleEndedIterator
        // Instead, we must collect it to a Vec then iterate over that.
        // For more info, see https://users.rust-lang.org/t/43401/2
        for (i, numeral) in number_string
            .chars()
            .collect::<Vec<char>>()
            .iter()
            .enumerate()
            .rev()
        {
            let numeral_rect = get_numeral_rect(*numeral);
            if (digits - i) % 3 == 1 && i != digits - 1 {
                sprite.set_texture_rect(&self.comma_rect);
                sprite.set_position(numeral_position);
                window.draw(&sprite);
                numeral_position.x -= numeral_rect.width as f32;
            }
            sprite.set_texture_rect(&numeral_rect);
            sprite.set_position(numeral_position);
            window.draw(&sprite);
            if i == 0 {
                break;
            }
            if (digits - i) % 3 == 0 {
                numeral_position.x -= self.comma_rect.width as f32;
                continue;
            }
            let numeral = number_string.as_bytes()[i - 1] as char;
            let numeral_rect = get_numeral_rect(numeral);
            numeral_position.x -= numeral_rect.width as f32;
        }
    }
}
