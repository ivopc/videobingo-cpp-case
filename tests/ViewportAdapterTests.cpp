#include "render/ViewportAdapter.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>

using bingo::render::ViewportAdapter;

namespace {

bool nearlyEqual(float a, float b, float epsilon = 0.001f) {
    return std::fabs(a - b) < epsilon;
}

void testMatchingAspectRatioFillsTheWindow() {
    const ViewportAdapter adapter(1280, 720);
    const sf::View view = adapter.computeView({1280u, 720u});
    const sf::FloatRect viewport = view.getViewport();

    assert(nearlyEqual(viewport.position.x, 0.0f));
    assert(nearlyEqual(viewport.position.y, 0.0f));
    assert(nearlyEqual(viewport.size.x, 1.0f));
    assert(nearlyEqual(viewport.size.y, 1.0f));
}

void testWiderWindowPillarboxes() {
    const ViewportAdapter adapter(1280, 720);
    const sf::View view = adapter.computeView({2000u, 900u});
    const sf::FloatRect viewport = view.getViewport();

    assert(nearlyEqual(viewport.size.y, 1.0f) && "full height used when pillarboxing");
    assert(viewport.size.x < 1.0f && "narrower-than-full width when pillarboxing");
    assert(nearlyEqual(viewport.position.x, (1.0f - viewport.size.x) / 2.0f) && "horizontally centered");
    assert(nearlyEqual(viewport.position.y, 0.0f));
}

void testTallerWindowLetterboxes() {
    const ViewportAdapter adapter(1280, 720);
    const sf::View view = adapter.computeView({900u, 900u});
    const sf::FloatRect viewport = view.getViewport();

    assert(nearlyEqual(viewport.size.x, 1.0f) && "full width used when letterboxing");
    assert(viewport.size.y < 1.0f && "shorter-than-full height when letterboxing");
    assert(nearlyEqual(viewport.position.y, (1.0f - viewport.size.y) / 2.0f) && "vertically centered");
    assert(nearlyEqual(viewport.position.x, 0.0f));
}

void testViewSizeAlwaysMatchesVirtualCanvas() {
    const ViewportAdapter adapter(1280, 720);
    const sf::View view = adapter.computeView({500u, 2000u});

    assert(nearlyEqual(view.getSize().x, 1280.0f) && "logical view size stays the virtual canvas, only the viewport changes");
    assert(nearlyEqual(view.getSize().y, 720.0f));
}

}

int main() {
    testMatchingAspectRatioFillsTheWindow();
    testWiderWindowPillarboxes();
    testTallerWindowLetterboxes();
    testViewSizeAlwaysMatchesVirtualCanvas();

    std::printf("ViewportAdapterTests: all tests passed\n");
    return 0;
}
