use std::{cmp::Ordering, env, fs, path::PathBuf};

#[derive(Debug)]
struct ColorCount<const N: usize> {
    count: usize,
    palette_index: u8,
    color: [u8; N],
}

#[derive(Debug)]
struct BitDepthError;

// This function determines the index of a certain color when mapped to the palette.
// Lower value means closer to index 0 in the palette. In indexed mode, just return the palette. 
fn color_priority<const N: usize>(c: &ColorCount<N>, color_mode: minipng::ColorType) -> usize {
    match color_mode {
        minipng::ColorType::Indexed => {
            c.palette_index as usize
        }
        _ => c.color.iter().map(|i| *i).fold(0, |acc, i| acc + i as usize) * (c.color[3] as usize)
        // _ => (0 as usize).wrapping_sub(c.count)
    }
}

fn get_color(bit_depth: u32, pixels: &[u8], index: usize) -> Result<[u8; 4], BitDepthError> {
    let res = match bit_depth {
        8 => [pixels[index], 0, 0, 0],
        16 => [pixels[index * 2], pixels[index * 2 + 1], 0, 0],
        24 => [pixels[index * 3], pixels[index * 3 + 1], pixels[index * 3 + 2], 0],
        32 => [pixels[index * 4], pixels[index * 4 + 1], pixels[index * 4 + 2], pixels[index * 4 + 3]],
        _ => {
            return Err(BitDepthError);
        }
    };
    Ok(res)
}

fn do_work() -> i32 {
    let mut args_it = env::args();

    args_it.next();

    let Some(path) = args_it.next() else {
        eprintln!("Path not provided");
        return 1;
    };

    let Ok(bytes) = fs::read(&path) else {
        eprintln!("Unable to read file from disk: \"{path}\"");
        return 1;
    };

    let Ok(header) = minipng::decode_png_header(&bytes) else {
        eprintln!("Unable to read png header of file: \"{path}\"");
        return 1;
    };
    let mut buffer = vec![0; header.required_bytes()];

    let Ok(img) = minipng::decode_png(&bytes, &mut buffer) else {
        eprintln!("Unable to decode png file: \"{path}\"");
        return 1;
    };

    let pixels = img.pixels();
    let color_type = img.color_type();
    let bit_depth = pixels.len() as u32 / img.width() / img.height() * 8;
    
    // Max 32-bit color
    let mut colors: Vec<ColorCount<4>> = vec![];

    for x in 0..img.width() {
        for y in 0..img.height() {
            let index = (y * img.width() + x) as usize;
            let Ok(color) = get_color(bit_depth, pixels, index) else {
                eprintln!("Error, bit depth is {bit_depth} is unsuppored. Consider using bit depth of 8");
                return 1;
            };

            let (palette, color) = if color_type == minipng::ColorType::Indexed {
                let index = color[0];
                (index, img.palette(index))
            } else { (0, color) };

            if let Some(ref mut color_count) = colors
                .iter_mut()
                .find(|color_count| color == color_count.color)
            {
                color_count.count += 1;
            } else {
                let color_count = ColorCount {
                    color,
                    palette_index: palette,
                    count: 0,
                };
                colors.push(color_count);
            }
        }
    }

    if colors.len() > 256 {
        eprintln!("Incorrect color count. Can handle 256 colors in palette at max. Counted {} different colors", colors.len());
        return 1;
    }

    colors.sort_by(|left, right| {
        if color_priority(left, color_type) > color_priority(right, color_type) {
            Ordering::Greater
        } else if color_priority(left, color_type) < color_priority(right, color_type) {
            Ordering::Less
        } else {
            Ordering::Equal
        }
    });

    eprintln!("Color buckets: {:?}", colors);



    // Assign palette index after sorting if image is non-indexed
    if color_type != minipng::ColorType::Indexed {
        colors.iter_mut().enumerate().for_each(|(i, c)| {
            c.palette_index = i as u8
        });
    }

    let max_palette_index = colors.iter().map(|c| c.palette_index).max().unwrap_or(0);

    let bpp = match max_palette_index {
        ..=1 => 1,
        ..=3 => 2,
        // ..=7 => 3,
        ..=15 => 4,
        // ..=31 => 5,
        // ..=63 => 6,
        // ..=127 => 7,
        ..=255 => 8,
    };

    let mut pixel_colors = vec![0u8; (img.width() * img.height()) as usize];

    let tile_width = 8;
    let tile_height = 8;

    let mut i = 0;
    for y in 0..img.height() / tile_height {
        for x in 0..img.width() / tile_width {
            for iy in 0..tile_height {
                for ix in 0..tile_width {
                    let index =
                        (y * img.width() * tile_height + iy * img.width() + x * tile_width + ix)
                            as usize;

                    // let color = pixels[index];
                    let Ok(color) = get_color(bit_depth, pixels, index) else {
                        eprintln!("Error, bit depth is {bit_depth} is unsuppored, unable to get color");
                        return 1;
                    };

                    let color = if color_type == minipng::ColorType::Indexed {
                        let index = color[0];
                        img.palette(index)
                    } else { color };

                    if let Some((color_index, color_count)) = colors
                        .iter()
                        .enumerate()
                        .find(|(_, color_count)| color == color_count.color)
                    {
                        pixel_colors[i] = color_index as u8;
                        pixel_colors[i] = color_count.palette_index as u8;
                        i += 1;
                    } else {
                        eprintln!("Could not find color {:?}", color);
                        return 1;
                    }
                }
            }
        }
    }

    let pixels_per_chunk = 8 / bpp;
    let pixel_bits_reduced = pixel_colors
        .chunks(pixels_per_chunk)
        .into_iter()
        .map(|chunks| match pixels_per_chunk {
            1 => chunks[0],
            2 => ((chunks[0] << 4) & 0b11110000) | ((chunks[1] << 0) & 0b00001111),
            4 => {
                ((chunks[0] << 6) & 0b11000000)
                    | ((chunks[1] << 4) & 0b00110000)
                    | ((chunks[2] << 2) & 0b00001100)
                    | ((chunks[3] << 0) & 0b00000011)
            }
            8 => {
                ((chunks[0] << 7) & 0b10000000)
                    | ((chunks[1] << 6) & 0b01000000)
                    | ((chunks[2] << 5) & 0b00100000)
                    | ((chunks[3] << 4) & 0b00010000)
                    | ((chunks[4] << 3) & 0b00001000)
                    | ((chunks[5] << 2) & 0b00000100)
                    | ((chunks[6] << 1) & 0b00000010)
                    | ((chunks[7] << 0) & 0b00000001)
            }
            _ => {
                panic!("Incorrect chunk size: {}", pixels_per_chunk);
            }
        })
        .collect::<Vec<_>>();

    println!(
        "Size: {}x{}, Type: {:?}, Bit-Depth: {bit_depth}, Bits per Pixel: {}, Uncompressed Size: {} bytes, Palette Size: {}",
        img.width(),
        img.height(),
        color_type,
        bpp,
        pixels.len(),
        max_palette_index,
    );

    let out_path: PathBuf = path.as_str().into();
    let out_path: String = out_path.with_extension("bin").to_string_lossy().into();
    let Ok(_) = fs::write(&out_path, &pixel_bits_reduced) else {
        eprintln!("Unable to save binary output to \"{out_path}\"");
        return 1;
    };
    eprintln!(
        "Saved bitmap to: \"{out_path}\", {} bytes",
        pixel_bits_reduced.len()
    );
    return 0;
}

fn main() {
    std::process::exit(do_work());
}
