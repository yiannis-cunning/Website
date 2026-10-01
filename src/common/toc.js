// Builds the table of contents inside <nav class="toc"> from the page's
// <h2 class="subhead"> and <h3> headings. Headings without an id get one
// generated from their text, so they can be linked to.
function slugify(text) {
    return text.toLowerCase().trim()
      .replace(/[^a-z0-9]+/g, '-')
      .replace(/^-|-$/g, '');
}

function buildToc() {
    const nav = document.querySelector('nav.toc');
    if (!nav) return;

    const headings = document.querySelectorAll('#maincontent h2.subhead, #maincontent h3');
    const list = document.createElement('ol');
    let subList = null;

    headings.forEach(h => {
      if (nav.contains(h)) return; // skip the "Contents" heading itself

      if (!h.id) h.id = slugify(h.textContent);

      const li = document.createElement('li');
      const a = document.createElement('a');
      a.className = 'goodlinks';
      a.href = '#' + h.id;
      a.textContent = h.textContent.trim();
      li.appendChild(a);

      if (h.tagName === 'H2') {
        list.appendChild(li);
        subList = null;
      } else {
        /*
        // h3: nest under the most recent h2
        if (!list.lastElementChild) return;
        if (!subList) {
          subList = document.createElement('ol');
          list.lastElementChild.appendChild(subList);
        }
        subList.appendChild(li);*/
      }
    });

    nav.appendChild(list);
}

buildToc();
