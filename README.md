# AdaptiveQuadMesh

Implementation in C++ of an [adaptive quad mesh simplification algorithm](media/publications/adaptive_quad_mesh_simplification.pdf)

# Representing meshes

## Structure

We have used a winged mesh structure (inspired by [this page](https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/model/winged-e.html)) to represent our object internally. The mesh is composed of a `list of edges`, a `list of vertices` and a `list of faces`.

The faces and the vertices are represented by an `id` and `one of the edges` connected to them.

The edges are represented by an `id`, its `vertex origin`, its `vertex destination`, its `left face`, its `right face`, its `surrounding edges` and its `reversed edge`.

## File format

To import our object, we're using the standard `obj` format. A triangular mesh is given as a string path to an object file and we begin by transforming it into a winged edge mesh using a technique adapted from [this](https://github.com/Jw-Jn/Mesh-Subdivision).
We then transform it into a quad dominant mesh by merging the edges....

<!-- TODO -->

## Operations

<!-- TODO -->

# First algorithm

From [this](media/publications/practical_quad_mesh_simplification.pdf)

## Triangle mesh to quad mesh

<!-- TODO -->

## Results

<!-- TODO -->

## Issues

<!-- TODO -->

# Improved algorithm

<!-- TODO -->

## Fitmaps

From [this](media/publications/automatic_construction_quad_using_fitmaps.pdf)

<!-- TODO -->

## Results

<!-- TODO -->

## Improvements from article

<!-- TODO -->

## What next

<!-- TODO -->