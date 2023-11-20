# German learning visualizer

This is a proof of concept to create flexible data visualization. It is also an attempt to store data close to the part that updates the UI.

This is the expecting result:

## UI object

```javascript

// The objects represent data that are floating in space
const ui = new UI(objects);

// render the ui with different perspective
ui.run(new NormalPerspective());
ui.run(new MapPerspective());
// Update the ui with new objects
ui.update(new_objects);

```

## Perspective

A perspective is necessary to access the data. Each data is wrapped with a perspective.

```javascript
const perspective = new Perspective();
perspective.action1(data);
```
