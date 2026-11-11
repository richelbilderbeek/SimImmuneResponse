#!/usr/bin/env python3
"""Play the WebAssembly game on the 'Play' page of a built website.

Usage: test_play_page.py <URL of the play page>

Checks that the game shows up in the browser at the expected spot,
that the simulation is running and that the player can produce a cell.
"""
import io
import sys
import time
from collections import Counter

from PIL import Image, ImageChops, ImageStat
from playwright.sync_api import sync_playwright

START_TIMEOUT_SECS = 60
# Size of one location in pixels, see src/sprite.h
PANE_WIDTH = 320
PANE_HEIGHT = 224
# Background colors of the locations, see src/sim_widget.cpp
BONE_MARROW = ((0, 0), (255, 196, 255))
BLOOD_STREAM = ((0, PANE_HEIGHT), (255, 196, 196))
LYMPH_NODE = ((PANE_WIDTH, PANE_HEIGHT), (255, 255, 196))


def most_common_color(shot, origin):
    """The most common color in a location."""
    x, y = origin
    pane = shot.crop((x, y, x + PANE_WIDTH, y + PANE_HEIGHT))
    return Counter(pane.getdata()).most_common(1)[0][0]


def shows_locations(shot):
    """Are the locations shown at the expected spots?"""
    return all(most_common_color(shot, origin) == color
               for origin, color in (BONE_MARROW, BLOOD_STREAM, LYMPH_NODE))


def count_non_background(shot, location):
    """Count the pixels in a location that are not the background."""
    (x, y), color = location
    pane = shot.crop((x, y, x + PANE_WIDTH, y + PANE_HEIGHT))
    return sum(1 for pixel in pane.getdata() if pixel != color)


def main():
    url = sys.argv[1]
    errors = []
    with sync_playwright() as playwright:
        browser = playwright.chromium.launch()
        page = browser.new_page(viewport={"width": 1280, "height": 1300})
        page.on("pageerror", lambda error: errors.append(str(error)))
        page.on("console", lambda m: print(f"console: {m.text}"))
        page.goto(url)
        frame = page.locator("iframe")

        def screenshot():
            data = frame.screenshot()
            return Image.open(io.BytesIO(data)).convert("RGB")

        print("Waiting for the game to show up")
        deadline = time.time() + START_TIMEOUT_SECS
        shot = screenshot()
        while time.time() < deadline and not shows_locations(shot):
            time.sleep(1)
            shot = screenshot()
        shot.save("play_page_start.png")
        assert shows_locations(shot), "Game not shown at the expected spot"
        assert not errors, errors

        print("Checking that the simulation is running")
        time.sleep(2)
        later = screenshot()
        moved = ImageStat.Stat(ImageChops.difference(
            shot.crop((0, PANE_HEIGHT, PANE_WIDTH, 2 * PANE_HEIGHT)),
            later.crop((0, PANE_HEIGHT, PANE_WIDTH, 2 * PANE_HEIGHT)))).mean
        assert sum(moved) > 0.0, "The viruses in the blood stream do not move"

        print("Producing a macrophage")
        before = count_non_background(later, BONE_MARROW)
        page.frame_locator("iframe").locator("body").click(
            position=find_button(later))
        time.sleep(1)
        after_shot = screenshot()
        after_shot.save("play_page_end.png")
        after = count_non_background(after_shot, BONE_MARROW)
        print(f"Non-background pixels in bone marrow: {before} -> {after}")
        assert after > before, "No macrophage produced in the bone marrow"
        assert not errors, errors
        browser.close()
    print("OK: the game can be played")


def find_button(shot):
    """Find the 'Produce macrophage' button, the topmost button below
    the locations, by looking for the first light row of pixels."""
    x = PANE_WIDTH // 3
    for y in range(2 * PANE_HEIGHT + 1, shot.height):
        if sum(shot.getpixel((x, y))) > 3 * 200:
            # Click a few pixels inside the button
            return {"x": x, "y": y + 8}
    raise AssertionError("Cannot find the 'Produce macrophage' button")


if __name__ == "__main__":
    main()
